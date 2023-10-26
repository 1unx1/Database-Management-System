#include "IndexHandler.h"

bool IndexHandler::_compare(CmpOp op, const Byte *key1, int pageID1, int slotID1,
                            const Byte *key2, int pageID2, int slotID2) const
{
    int cmp;
    switch (_fHeader._keyType)
    {
    case INT:
        cmp = *((int *)key1) - *((int *)key2);
        break;
    case FLOAT:
        cmp = *((float *)key1) - *((float *)key2);
        break;
    case VARCHAR:
        cmp = strncmp((const char *)key1, (const char *)key2, _fHeader._keyLen);
        break;
    default:
        break;
    }
    if (op == EQ)
        return cmp == 0 && pageID1 == pageID2 && slotID1 == slotID2;
    if (cmp)
        return cmp < 0;
    if (pageID1 != pageID2)
        return pageID1 - pageID2;
    return slotID1 < slotID2;
}

BTreeNode *IndexHandler::_loadNode(int nodePageID) const
{
    if (nodePageID == NULL_NODE)
        return nullptr;
    BTreeNode *node = new BTreeNode();
    // page to be write back
    BufType pageBuf = _bpm->getPage(_fileID, nodePageID, node->index);
    _bpm->access(node->index);
    node->pageBuf = (Byte *)pageBuf;
    memcpy(&node->_pHeader, pageBuf, sizeof(PageHeader));
    node->_keys = (Byte *)pageBuf + _fHeader._keyOffset;
    node->_pageIDs = (int *)((Byte *)pageBuf + _fHeader._pageIDOffset);
    node->_slotIDs = (int *)((Byte *)pageBuf + _fHeader._slotIDOffset);
    node->_childIDs = (int *)((Byte *)pageBuf + _fHeader._childIDOffset);
    return node;
}

void IndexHandler::_setEntry(BTreeNode *node, int idx, const Byte *key, int pageID, int slotID, int childID) const
{
    memcpy(node->_keys + idx * _fHeader._keyLen, key, _fHeader._keyLen);
    node->_pageIDs[idx] = pageID;
    node->_slotIDs[idx] = slotID;
    // printf("%d-----------pageid at 0x%x\n", idx, (Byte *)(node->_pageIDs + idx) + PAGE_SIZE - node->pageBuf);
    if (childID != NULL_NODE)
        node->_childIDs[idx] = childID;
    // if (idx == 510)
    //     printf("In SET, Node (%d, %d)\n", node->_pageIDs[0], node->_slotIDs[0]);
}

void IndexHandler::_writeNodeBack(BTreeNode *node, int nodePageID) const
{
    PageHeader temp = node->_pHeader;
    memcpy(node->pageBuf, &node->_pHeader, sizeof(PageHeader));
    _bpm->markDirty(node->index);
    _bpm->writeBack(node->index);
    BTreeNode *check = _loadNode(nodePageID);
    check->_pHeader = temp;
    memcpy(node->pageBuf, &node->_pHeader, sizeof(PageHeader));
    _bpm->markDirty(node->index);
    _bpm->writeBack(node->index);
}

void IndexHandler::_moveEntries(BTreeNode *dstNode, int dstIdx, BTreeNode *srcNode, int srcIdx, int n) const
{
    memmove(dstNode->_keys + dstIdx * _fHeader._keyLen, srcNode->_keys + srcIdx * _fHeader._keyLen, n * _fHeader._keyLen);
    // printf("move: from 0x%x to 0x%x.\n", srcNode->_pageIDs)
    memmove(dstNode->_pageIDs + dstIdx, srcNode->_pageIDs + srcIdx, n * 4);
    memmove(dstNode->_slotIDs + dstIdx, srcNode->_slotIDs + srcIdx, n * 4);
    memmove(dstNode->_childIDs + dstIdx, srcNode->_childIDs + srcIdx, n * 4);
}

bool IndexHandler::_overflow(int keyNum) const
{
    return keyNum > _fHeader._maxKeyNum;
}

bool IndexHandler::_underflow(int keyNum) const
{
    return keyNum < (_fHeader._maxKeyNum + 1) / 2;
}

void IndexHandler::_average(BTreeNode *leftNode, int leftNodePageID, BTreeNode *rightNode, int rightNodePageID) const
{
    int halfKeyNum = (leftNode->_pHeader._keyNum + rightNode->_pHeader._keyNum) / 2;
    if (leftNode->_pHeader._keyNum < rightNode->_pHeader._keyNum)
    {
        int n = halfKeyNum - leftNode->_pHeader._keyNum;
        // move entries from right node to left node
        _moveEntries(leftNode, leftNode->_pHeader._keyNum, rightNode, 0, n);
        for (int i = leftNode->_pHeader._keyNum; i <= halfKeyNum - 1; i++)
            if (leftNode->_childIDs[i] != NULL_NODE) // need change dad
            {
                BTreeNode *childNode = _loadNode(leftNode->_childIDs[i]);
                childNode->_pHeader._dadPageID = leftNodePageID;
                _writeNodeBack(childNode, leftNode->_childIDs[i]);
                delete childNode;
            }
        // move entries in right node
        _moveEntries(rightNode, 0, rightNode, n, n);
        // write 2 headers
        leftNode->_pHeader._keyNum = halfKeyNum;
        rightNode->_pHeader._keyNum -= n;
    }
    else
    {
        int n = halfKeyNum - rightNode->_pHeader._keyNum;
        // printf("First of left (%d, %d)\n", leftNode->_pageIDs[0], leftNode->_slotIDs[0]);
        // move entries in right node
        _moveEntries(rightNode, n, rightNode, 0, n);
        // move entries from left node to right node
        _moveEntries(rightNode, 0, leftNode, leftNode->_pHeader._keyNum - n, n);
        for (int i = 0; i <= n - 1; i++) // need change dad
            if (rightNode->_childIDs[i] != NULL_NODE)
            {
                BTreeNode *childNode = _loadNode(rightNode->_childIDs[i]);
                childNode->_pHeader._dadPageID = rightNodePageID;
                _writeNodeBack(childNode, rightNode->_childIDs[i]);
                delete childNode;
            }
        leftNode->_pHeader._keyNum -= n;
        rightNode->_pHeader._keyNum = halfKeyNum;
        // printf("First of left (%d, %d)\n", leftNode->_pageIDs[0], leftNode->_slotIDs[0]);
    }
}

void IndexHandler::_handleOverflow(BTreeNode *node, int nodePageID)
{
    // make a new node to share half entries
    // printf("%d has NEW BRO NODE PG ID: %d\n", nodePageID, _fHeader._nextFreePageID);
    int newNodePageID = _fHeader._nextFreePageID; // pageID 0 belongs to FileHeader
    _fHeader._nodeNum++;
    _fHeader._nextFreePageID++;
    BTreeNode *newNode = _loadNode(newNodePageID);
    _fHeaderChange = true; // flag for writing back
    newNode->_pHeader._isLeaf = node->_pHeader._isLeaf;
    newNode->_pHeader._keyNum = 0;
    newNode->_pHeader._nextPageID = newNode->_pHeader._prevPageID = NULL_NODE;
    // printf("OVERFLOW Node %d's keys:\n", nodePageID);
    // for (int i = 0; i <= node->_pHeader._keyNum - 1; i++)
    //     printf("%d, ", *((int *)(node->_keys + i * _fHeader._keyLen)));
    // printf("\n\n");
    // printf("First of Node (%d, %d)\n", node->_pageIDs[0], node->_slotIDs[0]);

    _average(node, nodePageID, newNode, newNodePageID);

    // printf("Node %d's keyNum: %d, Node %d's keyNum: %d", nodePageID, node->_pHeader._keyNum, newNodePageID, newNode->_pHeader._keyNum);
    // if need a new node to be served as a new root
    if (nodePageID == _fHeader._rootPageID)
    {
        // printf("SPLIT ROOT!\n");
        // printf("NEW ROOT NODE PG ID: %d\n", _fHeader._nextFreePageID);
        int newRootNodePageID = _fHeader._nextFreePageID; // pageID 0 belongs to FileHeader
        _fHeader._nodeNum++;
        _fHeader._nextFreePageID++;
        BTreeNode *newRootNode = _loadNode(newRootNodePageID);
        newRootNode->_pHeader._isLeaf = false;
        newRootNode->_pHeader._keyNum = 2;
        newRootNode->_pHeader._nextPageID = newRootNode->_pHeader._prevPageID = NULL_NODE;
        _setEntry(newRootNode, 0, node->_keys, node->_pageIDs[0], node->_slotIDs[0], nodePageID);
        _setEntry(newRootNode, 1, newNode->_keys, newNode->_pageIDs[0], newNode->_slotIDs[0], newNodePageID);
        _fHeader._rootPageID =
            node->_pHeader._dadPageID =
                newNode->_pHeader._dadPageID = newRootNodePageID;
        if (node->_pHeader._isLeaf)
        {
            node->_pHeader._nextPageID = newNodePageID;
            newNode->_pHeader._prevPageID = nodePageID;
        }
        _writeNodeBack(newRootNode, newRootNodePageID);
        delete newRootNode;

        // newRootNode = _loadNode(newRootNodePageID);
        // printf("NEW ROOT %d's keys:\n", newRootNodePageID);
        // for (int i = 0; i <= newRootNode->_pHeader._keyNum - 1; i++)
        //     printf("%d, ", *((int *)(newRootNode->_keys + i * _fHeader._keyLen)));
        // printf("IN FACT: %d, %d\n", *((int *)(node->_keys)), *((int *)(newNode->_keys)));
        // _bpm->writeBack(newRootNode->index);
        // delete newRootNode;
        // printf("\n\n");
        // exit(0);
    }
    else
    {
        newNode->_pHeader._dadPageID = node->_pHeader._dadPageID;
        // printf("NEW NODE's DAD IS %d\n", newNode->_pHeader._dadPageID);
        BTreeNode *dadNode = _loadNode(node->_pHeader._dadPageID);
        for (int i = 0; i <= dadNode->_pHeader._keyNum - 1; i++)
            if (dadNode->_childIDs[i] == nodePageID)
            {
                // printf("IT'S THE %d KID of %d\n", i + 1, dadNode->_pHeader._keyNum);
                _insertEntryInNode(dadNode, node->_pHeader._dadPageID, i + 1,
                                   newNode->_pageIDs[0], newNode->_slotIDs[0], newNode->_keys, newNode, newNodePageID);
                // for (int j = 0; j <= dadNode->_pHeader._keyNum - 1; j++)
                //     printf("DAD's %d KID is %d\n", j, dadNode->_childIDs[j]);
            }
        _writeNodeBack(dadNode, node->_pHeader._dadPageID);
        delete dadNode;
        if (node->_pHeader._isLeaf)
        {
            node->_pHeader._nextPageID = newNodePageID;
            newNode->_pHeader._prevPageID = nodePageID;
        }
    }
    PageHeader temp = newNode->_pHeader;
    _writeNodeBack(newNode, newNodePageID);
    // _writeNodeBack(node);
    delete newNode;
    // BTreeNode *check = _loadNode(newNodePageID);
    // check->_pHeader = temp;
    // _writeNodeBack(check);
    // delete check;

    // check = _loadNode(4);
    // printf("Node %d's keys:\n", 4);
    // for (int i = 0; i <= check->_pHeader._keyNum - 1; i++)
    //     printf("%d, ", *((int *)(check->_keys + i * _fHeader._keyLen)));
    // _bpm->writeBack(check->index);
    // delete check;
    // printf("\n\n");
    // check = _loadNode(newNodePageID);
    // printf("Node %d's keys:\n", newNodePageID);
    // for (int i = 0; i <= check->_pHeader._keyNum - 1; i++)
    //     printf("%d, ", *((int *)(check->_keys + i * _fHeader._keyLen)));
    // _bpm->writeBack(check->index);
    // delete check;
    // printf("\n");
    // if (nodePageID == 4)
    //     exit(0);
}

void IndexHandler::_handleUnderflow(BTreeNode *node, int nodePageID)
{
    if (nodePageID == _fHeader._rootPageID)
    {
        if (node->_pHeader._keyNum == 1 && !node->_pHeader._isLeaf)
        {
            BTreeNode *childNode = _loadNode(node->_childIDs[0]);
            childNode->_pHeader._dadPageID = NULL_NODE;
            _fHeader._nodeNum--; // flag for writing back
            _fHeader._rootPageID = node->_childIDs[0];
            _fHeaderChange = true;
        }
        return;
    }
    BTreeNode *dadNode = _loadNode(node->_pHeader._dadPageID);
    for (int i = 0; i <= dadNode->_pHeader._keyNum - 1; i++)
        if (dadNode->_childIDs[i] == nodePageID)
        {
            if (i - 1 >= 0) // try moving left node's entries to right
            {
                // printf("--------------1----------------\n");
                BTreeNode *broNode = _loadNode(dadNode->_childIDs[i - 1]);
                if (!_underflow(broNode->_pHeader._keyNum - 1)) // can move entry
                {
                    _average(broNode, dadNode->_childIDs[i - 1], node, nodePageID);
                    // due to the change of right node's min
                    _setEntry(dadNode, i, node->_keys, node->_pageIDs[0], node->_slotIDs[0]);
                    _writeNodeBack(broNode, dadNode->_childIDs[i - 1]);
                    delete broNode;
                    _writeNodeBack(dadNode, node->_pHeader._dadPageID);
                    delete dadNode;
                    return;
                }
                _bpm->writeBack(broNode->index);
                delete broNode;
            }
            if (i + 1 < dadNode->_pHeader._keyNum) // try moving right node's entries to left
            {
                // printf("--------------2----------------\n");
                BTreeNode *broNode = _loadNode(dadNode->_childIDs[i + 1]);
                if (!_underflow(broNode->_pHeader._keyNum - 1)) // can move entry
                {
                    _average(node, nodePageID, broNode, dadNode->_childIDs[i + 1]);
                    // due to the change of right node's min
                    _setEntry(dadNode, i + 1, broNode->_keys, broNode->_pageIDs[0], broNode->_slotIDs[0]);
                    _writeNodeBack(broNode, dadNode->_childIDs[i + 1]);
                    delete broNode;
                    _writeNodeBack(dadNode, node->_pHeader._dadPageID);
                    delete dadNode;
                    return;
                }
                _bpm->writeBack(broNode->index);
                delete broNode;
            }
            if (i - 1 >= 0) // try merging with left node
            {
                // printf("--------------3----------------\n");
                BTreeNode *broNode = _loadNode(dadNode->_childIDs[i - 1]);
                // move all entries in right node to left node
                _moveEntries(broNode, broNode->_pHeader._keyNum, node, 0, node->_pHeader._keyNum);
                for (int i = broNode->_pHeader._keyNum; i <= broNode->_pHeader._keyNum + node->_pHeader._keyNum - 1; i++)
                    if (broNode->_childIDs[i] != NULL_NODE) // need change dad
                    {
                        BTreeNode *childNode = _loadNode(broNode->_childIDs[i]);
                        childNode->_pHeader._dadPageID = dadNode->_childIDs[i - 1];
                        _writeNodeBack(childNode, broNode->_childIDs[i]);
                        delete childNode;
                    }
                broNode->_pHeader._keyNum += node->_pHeader._keyNum;
                _fHeader._nodeNum--; // flag for writing back
                _fHeaderChange = true;
                _deleteEntryInNode(dadNode, node->_pHeader._dadPageID, i);
                _writeNodeBack(broNode, dadNode->_childIDs[i - 1]);
                delete broNode;
                _writeNodeBack(dadNode, node->_pHeader._dadPageID);
                delete dadNode;
                return;
            }
            if (i + 1 < dadNode->_pHeader._keyNum) // try merging with right node
            {
                // printf("--------------4----------------\n");
                BTreeNode *broNode = _loadNode(dadNode->_childIDs[i + 1]);
                // move all entries in right node to left node
                _moveEntries(node, node->_pHeader._keyNum, broNode, 0, broNode->_pHeader._keyNum);
                for (int i = node->_pHeader._keyNum; i <= broNode->_pHeader._keyNum + node->_pHeader._keyNum - 1; i++)
                    if (node->_childIDs[i] != NULL_NODE) // need change dad
                    {
                        BTreeNode *childNode = _loadNode(node->_childIDs[i]);
                        childNode->_pHeader._dadPageID = nodePageID;
                        _writeNodeBack(childNode, node->_childIDs[i]);
                        delete childNode;
                    }
                node->_pHeader._keyNum += broNode->_pHeader._keyNum;
                _fHeader._nodeNum--; // flag for writing back
                _fHeaderChange = true;
                _deleteEntryInNode(dadNode, node->_pHeader._dadPageID, i + 1);
                // don't write back broNode, due to it disappeared
                delete broNode;
                _writeNodeBack(dadNode, node->_pHeader._dadPageID);
                delete dadNode;
                return;
            }
            _bpm->writeBack(dadNode->index);
            delete dadNode;
            return;
        }
    _bpm->writeBack(dadNode->index);
    delete dadNode;
    return;
}

void IndexHandler::_insertEntryInNode(BTreeNode *node, int nodePageID, int idx, int pageID, int slotID, const Byte *key, BTreeNode *childNode, int childID)
{
    // i -> i + 1, vacate for the new entry
    if (idx != node->_pHeader._keyNum)
    {
        // printf("moved.\n");
        _moveEntries(node, idx + 1, node, idx, node->_pHeader._keyNum - idx);
    }
    node->_pHeader._keyNum++;
    // if (idx == 510)
    // {
    // printf("First of Node (%d, %d)\n", node->_pageIDs[0], node->_slotIDs[0]);
    // printf("Set to (%d, %d)\n", pageID, slotID);
    // }
    _setEntry(node, idx, key, pageID, slotID, childID);
    // if (idx == 510)
    //     printf("First of Node (%d, %d)\n", node->_pageIDs[0], node->_slotIDs[0]);
    // if node's children are leaves, need to change their link
    if (!node->_pHeader._isLeaf)
    {
        BTreeNode *firstChildNode = _loadNode(node->_childIDs[0]);
        if (firstChildNode->_pHeader._isLeaf) // need to change link
        {
            if (idx == 0) // new front
            {
                BTreeNode *nextNode = _loadNode(node->_childIDs[1]);
                nextNode->_pHeader._prevPageID = childID;
                childNode->_pHeader._nextPageID = node->_childIDs[1];
                if (nextNode->_pHeader._prevPageID != NULL_NODE)
                {
                    BTreeNode *prevNode = _loadNode(nextNode->_pHeader._prevPageID);
                    prevNode->_pHeader._nextPageID = childID;
                    _writeNodeBack(prevNode, nextNode->_pHeader._prevPageID);
                    delete prevNode;
                }
                childNode->_pHeader._prevPageID = nextNode->_pHeader._prevPageID;
                _writeNodeBack(nextNode, node->_childIDs[1]);
                delete nextNode;
            }
            else if (idx == node->_pHeader._keyNum - 1) // new tail
            {
                BTreeNode *prevNode = _loadNode(node->_childIDs[node->_pHeader._keyNum - 2]);
                childNode->_pHeader._prevPageID = node->_childIDs[node->_pHeader._keyNum - 2];
                childNode->_pHeader._nextPageID = prevNode->_pHeader._nextPageID;
                prevNode->_pHeader._nextPageID = childID;
                if (prevNode->_pHeader._nextPageID != NULL_NODE)
                {
                    BTreeNode *nextNode = _loadNode(prevNode->_pHeader._nextPageID);
                    nextNode->_pHeader._prevPageID = childID;
                    _writeNodeBack(nextNode, prevNode->_pHeader._nextPageID);
                    delete nextNode;
                }
                _writeNodeBack(prevNode, node->_childIDs[node->_pHeader._keyNum - 2]);
                delete prevNode;
            }
            else
            {
                BTreeNode *prevNode = _loadNode(node->_childIDs[idx - 1]);
                BTreeNode *nextNode = _loadNode(node->_childIDs[idx + 1]);
                prevNode->_pHeader._nextPageID = childID;
                nextNode->_pHeader._prevPageID = childID;
                childNode->_pHeader._prevPageID = node->_childIDs[idx - 1];
                childNode->_pHeader._nextPageID = node->_childIDs[idx + 1];
                _writeNodeBack(prevNode, node->_childIDs[idx - 1]);
                delete prevNode;
                _writeNodeBack(nextNode, node->_childIDs[idx + 1]);
                delete nextNode;
            }
        }
        _bpm->writeBack(firstChildNode->index);
        delete firstChildNode;
    }
    // _writeNodeBack(node);
    // ancestors have been modified when call findEntry
    if (_overflow(node->_pHeader._keyNum))
        _handleOverflow(node, nodePageID);
}

void IndexHandler::_deleteEntryInNode(BTreeNode *node, int nodePageID, int idx)
{
    int deletedPageID = node->_childIDs[idx];
    // delete entry at idx, i <- i + 1
    if (idx != node->_pHeader._keyNum - 1)
        _moveEntries(node, idx, node, idx + 1, node->_pHeader._keyNum - 1 - idx);
    node->_pHeader._keyNum--;
    // if node's children are leaves, need to change their link
    if (!node->_pHeader._isLeaf)
    {
        BTreeNode *deletedChildNode = _loadNode(deletedPageID);
        if (deletedChildNode->_pHeader._isLeaf) // need to change link
        {
            if (deletedChildNode->_pHeader._prevPageID != NULL_NODE)
            {
                BTreeNode *prevNode = _loadNode(deletedChildNode->_pHeader._prevPageID);
                prevNode->_pHeader._nextPageID = deletedChildNode->_pHeader._nextPageID;
                _writeNodeBack(prevNode, deletedChildNode->_pHeader._prevPageID);
                delete prevNode;
            }
            if (deletedChildNode->_pHeader._nextPageID != NULL_NODE)
            {
                BTreeNode *nextNode = _loadNode(deletedChildNode->_pHeader._nextPageID);
                nextNode->_pHeader._prevPageID = deletedChildNode->_pHeader._prevPageID;
                _writeNodeBack(nextNode, deletedChildNode->_pHeader._nextPageID);
                delete nextNode;
            }
        }
        _bpm->writeBack(deletedChildNode->index);
        delete deletedChildNode;
    }
    // _writeNodeBack(node);
    if (idx == 0 && nodePageID != _fHeader._rootPageID)
    {
        BTreeNode *childNode = node;
        int childNodePageID = nodePageID;
        while (childNodePageID != _fHeader._rootPageID)
        {
            BTreeNode *dadNode = _loadNode(childNode->_pHeader._dadPageID);
            if (dadNode->_childIDs[0] == childNodePageID) // need to go far
            {
                _setEntry(dadNode, 0, childNode->_keys, childNode->_pageIDs[0], childNode->_slotIDs[0]);
                childNodePageID = childNode->_pHeader._dadPageID;
                if (childNode != node)
                {
                    _writeNodeBack(childNode, childNodePageID);
                    delete childNode;
                }
                childNode = dadNode;
            }
            else // should stop, no more ancestor to be modify
            {
                for (int i = 1; i <= dadNode->_pHeader._keyNum - 1; i++)
                    if (dadNode->_childIDs[i] == childNodePageID)
                    {
                        _setEntry(dadNode, i, childNode->_keys, childNode->_pageIDs[0], childNode->_slotIDs[0]);
                        break;
                    }
                _writeNodeBack(dadNode, childNode->_pHeader._dadPageID);
                delete dadNode;
                if (childNode != node)
                {
                    _writeNodeBack(childNode, childNodePageID);
                    delete childNode;
                }
                break;
            }
        }
        if (childNodePageID == _fHeader._rootPageID)
        {
            _writeNodeBack(childNode, childNodePageID);
            delete childNode; // childNode == rootNode now
        }
    }
    if (_underflow(node->_pHeader._keyNum))
        _handleUnderflow(node, nodePageID);
}

// for Scanner
bool IndexHandler::_getFirstEntry(int &targetPageID, int &idx, Byte *firstKey) const
{
    int nodePageID = _fHeader._rootPageID;
    while (true)
    {
        BTreeNode *node = _loadNode(nodePageID);
        if (!node->_pHeader._isLeaf)
        {
            nodePageID = node->_childIDs[0];
            _bpm->writeBack(node->index);
            delete node;
        }
        else
        {
            if (node->_pHeader._keyNum == 0) // no entry
                return false;
            targetPageID = nodePageID;
            idx = 0;
            memcpy(firstKey, node->_keys, _fHeader._keyLen);
            _bpm->writeBack(node->index);
            delete node;
            return true;
        }
    }
}

bool IndexHandler::_getNextEntry(int &targetPageID, int &idx, Byte *nextKey) const
{
    BTreeNode *node = _loadNode(targetPageID);
    // printf("idx: %d, node %d's keynum: %d\n", idx, targetPageID, node->_pHeader._keyNum);
    if (idx <= node->_pHeader._keyNum - 1) // the next entry is in the same node
    {
        idx++;
        memcpy(nextKey, node->_keys + idx * _fHeader._keyLen, _fHeader._keyLen);
        _bpm->writeBack(node->index);
        delete node;
        return true;
    }
    // printf("node %d's next: %d\n", targetPageID, node->_pHeader._nextPageID);
    // the next entry is in the next node
    if (node->_pHeader._nextPageID != NULL_NODE)
    {
        targetPageID = node->_pHeader._nextPageID;
        idx = 0;
        BTreeNode *nextNode = _loadNode(targetPageID);
        memcpy(nextKey, nextNode->_keys, _fHeader._keyLen);
        _bpm->writeBack(nextNode->index);
        delete nextNode;
        _bpm->writeBack(node->index);
        delete node;
        return true;
    }
    // no next entry
    _bpm->writeBack(node->index);
    delete node;
    return false;
}

void IndexHandler::_getRID(int nodePageID, int idx, RID &rid) const
{
    BTreeNode *node = _loadNode(nodePageID);
    rid._pageID = node->_pageIDs[idx];
    // printf("node's 1st key %d: pid :%d, sid :%d\n", *((int *)(node->_keys)), node->_pageIDs[0], node->_slotIDs[0]);
    // printf("node's 2nd key %d: pid :%d, sid :%d\n", *((int *)(node->_keys + 4)), node->_pageIDs[1], node->_slotIDs[1]);
    // printf("node's 3rd key %d: pid :%d, sid :%d\n", *((int *)(node->_keys + 8)), node->_pageIDs[2], node->_slotIDs[2]);
    // printf("node's 4th key %d: pid :%d, sid :%d\n", *((int *)(node->_keys + 12)), node->_pageIDs[3], node->_slotIDs[3]);
    rid._slotID = node->_slotIDs[idx];
    _bpm->writeBack(node->index);
    delete node;
}

void IndexHandler::init(int fileID, BufPageManager *bpm, FileManager *fm)
{
    _fileID = fileID;
    _bpm = bpm;
    _fm = fm;
    // load FileHeader on page 0
    int index;
    BufType pageBuf = bpm->getPage(_fileID, META_PAGE_ID, index);
    memcpy(&_fHeader, pageBuf, sizeof(IDX_FileHeader));
    bpm->access(index);
    _fHeaderChange = false;
}

// when insert == false, find the max entry <= (key, rid), may be not existing (idx = -1)
// when insert == true, find which idx (key, rid) should be
void IndexHandler::findEntry(const RID &rid, const Byte *key, bool insert, int &idx, int &targetPageID, bool &last, Byte *getKey) const
{
    int nodePageID = _fHeader._rootPageID;
    // printf("node num: %d, root: %d\n", _fHeader._nodeNum, _fHeader._rootPageID);
    while (true)
    {
        BTreeNode *node = _loadNode(nodePageID);
        // if (nodePageID == 4)
        // {
        //     for (int i = 1; i <= 3; i++)
        //     {
        //         BTreeNode *n = _loadNode(i);
        //         printf("%d IS LEAF? %d\n", i, n->_pHeader._isLeaf);
        //         _bpm->writeBack(n->index);
        //     }
        //     printf("4 IS LEAF? %d\n", node->_pHeader._isLeaf);
        // }
        if (!node->_pHeader._isLeaf)
        {
            // printf("INNER NODE.\n");
            _bpm->access(node->index);
            if (_compare(LT, key, rid._pageID, rid._slotID, node->_keys, node->_pageIDs[0], node->_slotIDs[0]))
            // entry to find < min entry
            {
                // printf("cmpkey < node's min entry %d.\n", *((int *)node->_keys));
                if (insert) // if find for inserting
                {
                    // if (node->_childIDs[0] == 4)
                    //     printf("-------------1--------------%d -> %d, root %d\n", nodePageID, node->_childIDs[0], _fHeader._rootPageID);
                    nodePageID = node->_childIDs[0];
                    _setEntry(node, 0, key, rid._pageID, rid._slotID); // modify all ancestors
                }
                else // find for deleting, but mismatch
                {
                    idx = -1;
                    delete node;
                    return;
                }
            }
            else
            {
                // printf("cmpkey >= node's min entry %d.\n", *((int *)node->_keys));
                bool find = false;
                for (int i = 1; i <= node->_pHeader._keyNum - 1; i++)
                {
                    // printf("KEY AT %d: %d\n", i, *((int *)(node->_keys + i * _fHeader._keyLen)));
                    if (_compare(LT, key, rid._pageID, rid._slotID,
                                 node->_keys + i * _fHeader._keyLen,
                                 node->_pageIDs[i], node->_slotIDs[i]))
                    {
                        nodePageID = node->_childIDs[i - 1]; // the min entry <= entry to find
                        find = true;
                        break;
                    }
                }
                if (!find) // entry to find >= max entry
                    nodePageID = node->_childIDs[node->_pHeader._keyNum - 1];
            }
            delete node;
        }
        else
        {
            targetPageID = nodePageID;
            // printf("leaf key num: %d\n", node->_pHeader._keyNum);
            if (node->_pHeader._keyNum == 0 || _compare(LT, key, rid._pageID, rid._slotID, node->_keys, node->_pageIDs[0], node->_slotIDs[0]))
            {
                if (insert)
                    idx = 0;
                else
                {
                    idx = -1;
                    _bpm->writeBack(node->index);
                    delete node;
                    return;
                }
            }
            else
            {
                // printf("cmpkey >= leaf's min entry %d.\n", *((int *)node->_keys));
                bool find = false;
                for (int i = 1; i <= node->_pHeader._keyNum - 1; i++)
                    if (_compare(LT, key, rid._pageID, rid._slotID,
                                 node->_keys + i * _fHeader._keyLen,
                                 node->_pageIDs[i], node->_slotIDs[i]))
                    {
                        idx = insert ? i : i - 1;
                        find = true;
                        break;
                    }
                if (!find)
                    idx = insert ? node->_pHeader._keyNum : node->_pHeader._keyNum - 1;
            }
            if (getKey != nullptr && idx < node->_pHeader._keyNum) // for Scanner to compare key
                memcpy(getKey, node->_keys + idx * _fHeader._keyLen, _fHeader._keyLen);
            last = (idx == node->_pHeader._keyNum);
            _bpm->writeBack(node->index);
            delete node;
            return;
        }
    }
}

bool IndexHandler::insertEntry(const RID &rid, const Byte *key)
{
    int idx, nodePageID;
    bool last;
    // printf("-------------------SAFE-------------------\n");
    findEntry(rid, key, true, idx, nodePageID, last);
    BTreeNode *node = _loadNode(nodePageID);
    if (_compare(EQ, node->_keys + idx * _fHeader._keyLen, node->_pageIDs[idx], node->_slotIDs[idx],
                 key, rid._pageID, rid._slotID))
    {
        _bpm->writeBack(node->index);
        delete node;
        // printf("Already exist.\n");
        return false;
    }
    _insertEntryInNode(node, nodePageID, idx, rid._pageID, rid._slotID, key, nullptr, NULL_NODE);
    _writeNodeBack(node, nodePageID);
    delete node;
    // if (nodePageID <= 2)
    // {
    // printf("MAX NUM: %d\n\n", _fHeader._maxKeyNum);
    // BTreeNode *check = _loadNode(1);
    // printf("Node 1's %d keys:\n", check->_pHeader._keyNum);
    // for (int i = 0; i <= check->_pHeader._keyNum - 1; i++)
    //     printf("(%d, %d), ", check->_pageIDs[i], check->_slotIDs[i]);
    // _bpm->writeBack(check->index);
    // printf("\n\n");
    // check = _loadNode(2);
    // printf("Node 2's %d keys:\n", check->_pHeader._keyNum);
    // for (int i = 0; i <= check->_pHeader._keyNum - 1; i++)
    //     printf("(%d, %d), ", check->_pageIDs[i], check->_slotIDs[i]);
    // _bpm->writeBack(check->index);
    // printf("\n\n");
    // check = _loadNode(4);
    // printf("Node 4's %d keys:\n", check->_pHeader._keyNum);
    // for (int i = 0; i <= check->_pHeader._keyNum - 1; i++)
    //     printf("%d, ", *((int *)(check->_keys + i * _fHeader._keyLen)));
    // _bpm->writeBack(check->index);
    // printf("\n\n");
    // check = _loadNode(5);
    // printf("Node 5's %d keys:\n", check->_pHeader._keyNum);
    // for (int i = 0; i <= check->_pHeader._keyNum - 1; i++)
    //     printf("%d, ", *((int *)(check->_keys + i * _fHeader._keyLen)));
    // _bpm->writeBack(check->index);
    // printf("\n\n");
    // check = _loadNode(6);
    // printf("Node 6's %d keys:\n", check->_pHeader._keyNum);
    // for (int i = 0; i <= check->_pHeader._keyNum - 1; i++)
    //     printf("%d, ", *((int *)(check->_keys + i * _fHeader._keyLen)));
    // _bpm->writeBack(check->index);
    // printf("\n\n");
    // check = _loadNode(7);
    // printf("Node 7's %d keys:\n", check->_pHeader._keyNum);
    // for (int i = 0; i <= check->_pHeader._keyNum - 1; i++)
    //     printf("%d, ", *((int *)(check->_keys + i * _fHeader._keyLen)));
    // _bpm->writeBack(check->index);
    // printf("\n\n");
    // if (nodePageID == 2)
    //     exit(0);
    // }
    return true;
}

bool IndexHandler::deleteEntry(const RID &rid, const Byte *key)
{
    int idx, nodePageID;
    bool last;
    findEntry(rid, key, false, idx, nodePageID, last);
    if (idx != -1)
    {
        BTreeNode *node = _loadNode(nodePageID);
        if (_compare(EQ, node->_keys + idx * _fHeader._keyLen, node->_pageIDs[idx], node->_slotIDs[idx],
                     key, rid._pageID, rid._slotID))
        {
            _deleteEntryInNode(node, nodePageID, idx);
            _writeNodeBack(node, nodePageID);
            delete node;
            // printf("Delete in idx well.\n");
            return true;
        }
        _bpm->writeBack(node->index);
        delete node;
    }
    // printf("No matched KEY you wanna delete in this table.\n");
    return false;
}
