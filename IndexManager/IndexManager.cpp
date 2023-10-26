#include "IndexManager.h"

bool IndexManager::createIndexFile(const char *fileName, int idxOfIndex, ColumnType attrType, int attrLen) const
{
    // create and open new file for index
    std::string indexFileName = NameUtil::indexFileName(fileName, idxOfIndex);
    if (!_fm->createFile(indexFileName.c_str()))
        return false;
    int fileID;
    if (!_fm->openFile(indexFileName.c_str(), fileID))
        return false;
    // write FileHeader on pageID 0
    IDX_FileHeader fHeader;
    fHeader._keyType = attrType;
    fHeader._keyLen = attrLen;
    fHeader._rootPageID = 1; // pageID 0 belongs to FileHeader
    fHeader._nodeNum = 1;    // only root node, not contains page 0 of FileHeader
    fHeader._nextFreePageID = 2;
    // entry = (key, pageID, slotID, childID), keyLen + 3 * sizeof(int)
    // Page Structrue: [ PageHeader | keys | pageIDs | slotIDs | childIDs ]
    fHeader._maxKeyNum = (PAGE_SIZE - sizeof(PageHeader)) / (attrLen + 12) - 1;
    fHeader._keyOffset = sizeof(PageHeader);
    // maxKeyNum + 1 to deal with sometimes overflow, set last key but on fisrt pageID, set last pageID but on first slotID
    // so maxKeyNum should be 1 smaller than number we compute
    fHeader._pageIDOffset = fHeader._keyOffset + attrLen * (fHeader._maxKeyNum + 1);
    fHeader._slotIDOffset = fHeader._pageIDOffset + 4 * (fHeader._maxKeyNum + 1);
    fHeader._childIDOffset = fHeader._slotIDOffset + 4 * (fHeader._maxKeyNum + 1);
    int index;
    BufType pageBuf = _bpm->getPage(fileID, META_PAGE_ID, index);
    memset(pageBuf, 0, PAGE_SIZE);
    memcpy(pageBuf, &fHeader, sizeof(IDX_FileHeader));
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    // write root node on pageID 1
    PageHeader pHeader;
    pHeader._isLeaf = true;
    pHeader._prevPageID =
        pHeader._nextPageID =
            pHeader._dadPageID = NULL_NODE;
    pHeader._keyNum = 0;
    pageBuf = _bpm->getPage(fileID, 1, index);
    memset(pageBuf, 0, PAGE_SIZE);
    memcpy(pageBuf, &pHeader, sizeof(PageHeader));
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    // close file
    _fm->closeFile(fileID);
    return true;
}

bool IndexManager::destroyIndexFile(const char *fileName, int idxOfIndex) const
{
    _bpm->close();
    std::string rmCmd = "rm " + NameUtil::indexFileName(fileName, idxOfIndex);
    system(rmCmd.c_str());
    return true;
}

bool IndexManager::openIndexFile(const char *fileName, int idxOfIndex, IndexHandler &idxHandler) const
{
    // get fileID
    int fileID;
    std::string indexFileName = NameUtil::indexFileName(fileName, idxOfIndex);
    if (!_fm->openFile(indexFileName.c_str(), fileID))
        return false;
    // initialize handler
    idxHandler.init(fileID, _bpm, _fm);
    return true;
}

bool IndexManager::closeIndexFile(IndexHandler &idxHandler) const
{
    if (idxHandler._fHeaderChange) // page 0 which stored FileHeader need write back
    {
        int index;
        BufType pageBuf = _bpm->getPage(idxHandler._fileID, META_PAGE_ID, index);
        memcpy(pageBuf, &idxHandler._fHeader, sizeof(IDX_FileHeader));
        _bpm->markDirty(index);
        _bpm->writeBack(index);
    }
    _bpm->close(); // write back all pages which need to be write back
    // printf("close idxfile.\n");
    _fm->closeFile(idxHandler._fileID);
    return true;
}
