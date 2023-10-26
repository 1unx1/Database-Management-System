#include "IndexScanner.h"

// to find key op cmpKey is true
void IndexScanner::openScan(IndexHandler *idxHandler, const Byte *cmpKey, CmpOp op)
{
    // initialization
    _idxHandler = idxHandler;
    _firstTime = true;
    _noMore = true;
    _cmpKey = new Byte[_idxHandler->_fHeader._keyLen];
    memcpy(_cmpKey, cmpKey, _idxHandler->_fHeader._keyLen);
    _findKey = new Byte[_idxHandler->_fHeader._keyLen];
    _op = op;
}

bool IndexScanner::getNextEntry(RID &rid)
{
    if (_firstTime) // if found, found the min entry = (_cmpKey, min RID)
    {
        _firstTime = false;
        if (_op == EQ)
        {
            bool last;
            // pass RID(-1, -1), so (_cmpKey, -1, -1) < any (_cmpKey, pageID, slotID)
            _idxHandler->findEntry(RID(-1, -1), _cmpKey, true, _idx, _leafPageID, last, _findKey);
            // printf("findkey %d, at pid %d, idx %d, last is %d\n", *((int *)_findKey), _leafPageID, _idx, last);
            if (last)
                if (!_idxHandler->_getNextEntry(_leafPageID, _idx, _findKey))
                {
                    // printf("FALSE 1.\n");
                    _noMore = true;
                    return false;
                }
            // printf(">%d<", *((int *)(_findKey)));
        }
        else if (!_idxHandler->_getFirstEntry(_leafPageID, _idx, _findKey))
        {
            _noMore = true;
            return false;
        };
        // printf("findkey %d, at pid %d, idx %d\n", *((int *)_findKey), _leafPageID, _idx);
        if (CmpUtil::match(_idxHandler->_fHeader._keyType, _findKey, _op, _cmpKey, _idxHandler->_fHeader._keyLen))
        {
            _idxHandler->_getRID(_leafPageID, _idx, rid);
            // printf("%d, (%d, %d) at (%d, %d)\n", *((int *)_findKey), rid._pageID, rid._slotID, _leafPageID, _idx);
            _noMore = false;
            return true;
        }
        if (_op == NE || _op == GT || _op == GE) // when op belongs to these, can not decide whether no more or not
        {
            while (_idxHandler->_getNextEntry(_leafPageID, _idx, _findKey))
                if (CmpUtil::match(_idxHandler->_fHeader._keyType, _findKey, _op, _cmpKey, _idxHandler->_fHeader._keyLen))
                {
                    _idxHandler->_getRID(_leafPageID, _idx, rid);
                    _noMore = false;
                    return true;
                }
            // printf("FALSE 2.\n");
            _noMore = true;
            return false;
        }
        else
        {
            // printf("FALSE 3.\n");
            _noMore = true;
            return false;
        }
    }
    else
    {
        if (_noMore) // impossible to find more matched entries if we failed for the first time
            return false;
        // try finding more
        if (!_idxHandler->_getNextEntry(_leafPageID, _idx, _findKey))
        {
            _noMore = true;
            return false;
        }
        if (CmpUtil::match(_idxHandler->_fHeader._keyType, _findKey, _op, _cmpKey, _idxHandler->_fHeader._keyLen))
        {
            _idxHandler->_getRID(_leafPageID, _idx, rid);
            // printf("%d, (%d, %d) at (%d, %d)\n", *((int *)_findKey), rid._pageID, rid._slotID, _leafPageID, _idx);
            _noMore = false;
            return true;
        }
        if (_op == NE || _op == GT || _op == GE) // when op belongs to these, can not decide whether no more or not
        {
            while (_idxHandler->_getNextEntry(_leafPageID, _idx, _findKey))
                if (CmpUtil::match(_idxHandler->_fHeader._keyType, _findKey, _op, _cmpKey, _idxHandler->_fHeader._keyLen))
                {
                    _idxHandler->_getRID(_leafPageID, _idx, rid);
                    _noMore = false;
                    return true;
                }
            _noMore = true;
            return false;
        }
        else
        {
            _noMore = true;
            return false;
        }
    }
}

bool IndexScanner::closeScan() const
{
    delete[] _cmpKey;
    delete[] _findKey;
    return true;
}
