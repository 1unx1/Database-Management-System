#include "RecordHandler.h"

// Page Structrue: [ usedSlotNum | nextFreePageID | slots | n nullBitMaps | existBitMap ]
// slot: [ nullBitMap(nullLen) | column 1 | column 2 | ... | column n ]
// keep page 0 stay in buffer, write it back when manager close file

bool RecordHandler::_getBit(Byte *bitMap, int idx) const
{
    return (bitMap[-(idx / 8)] >> (idx % 8)) & 0x1;
}

void RecordHandler::_setBitOne(Byte *bitMap, int idx) const
{
    bitMap[-(idx / 8)] |= 0x1 << (idx % 8);
}

void RecordHandler::_setBitZero(Byte *bitMap, int idx) const
{
    bitMap[-(idx / 8)] &= ~(0x1 << (idx % 8));
}

bool RecordHandler::_getFirstZeroBit(Byte *bitMap, int &idx) const
{
    idx = 0;
    while (idx < _fHeader._maxSlotNum)
    {
        if (!_getBit(bitMap, idx))
            return true;
        idx++;
    }
    return false;
}

bool RecordHandler::_getNextOneBit(Byte *bitMap, int startIdx, int &idx) const
{
    idx = startIdx;
    while (idx < _fHeader._maxSlotNum)
    {
        if (_getBit(bitMap, idx))
            return true;
        idx++;
    }
    return false;
}
// for Scanner
bool RecordHandler::_getNextRec(int startPageID, int startSlotID, RID &rid, Byte *dataDst) const
{
    BufType pageBuf;
    // find in all pages starting at startPageID
    bool atStartPage = true;
    while (true)
    {
        if (startPageID > _fHeader._pageNum - 1) // no more pages
            return false;
        int index;
        pageBuf = _bpm->getPage(_fileID, startPageID, index);
        Byte *bitMap = (Byte *)pageBuf + PAGE_SIZE - 1;
        _bpm->access(index); // read only
        if (!atStartPage)
            startSlotID = 0;
        int slotID;
        if (_getNextOneBit(bitMap, startSlotID, slotID))
        // start at slotID = 0 if not at startPage else start at specified slotID
        {
            rid._pageID = startPageID;
            rid._slotID = slotID;
            break;
        }
        startPageID++; // try finding in the next page
        atStartPage = false;
    }
    Byte *dataSrc = (Byte *)pageBuf + PAGE_HEADER_LEN + rid._slotID * _fHeader._slotLen;
    memcpy(dataDst, dataSrc, _fHeader._slotLen);
    return true;
}

bool RecordHandler::_getRecNum(int pageID, int &recNum) const
{
    if (pageID > _fHeader._pageNum - 1) // no more pages
        return false;
    int index;
    BufType pageBuf = _bpm->getPage(_fileID, pageID, index);
    _bpm->access(index); // read only
    recNum = pageBuf[USED_SLOT_NUM];
    return true;
}

void RecordHandler::init(int fileID, BufPageManager *bpm, FileManager *fm)
{
    _fileID = fileID;
    _bpm = bpm;
    _fm = fm;
    // load FileHeader on meta page
    int index;
    BufType pageBuf = bpm->getPage(_fileID, META_PAGE_ID, index);
    memcpy(&_fHeader, pageBuf, sizeof(RM_FileHeader));
    bpm->access(index);
    _fHeaderChange = false;
}

bool RecordHandler::accessRec(const RID &rid, Byte *dataDst) const
{
    int index;
    BufType pageBuf = _bpm->getPage(_fileID, rid._pageID, index);
    _bpm->access(index); // read only
    Byte *bitMap = (Byte *)pageBuf + PAGE_SIZE - 1;
    if (!_getBit(bitMap, rid._slotID)) // not exist
    {
        // printf("No matched RID you wanna access in this table.\n");
        return false;
    }
    Byte *dataSrc = (Byte *)pageBuf + PAGE_HEADER_LEN + rid._slotID * _fHeader._slotLen;
    memcpy(dataDst, dataSrc, _fHeader._slotLen);
    return true;
}

bool RecordHandler::insertRec(const Byte *dataSrc, RID &rid)
{
    if (_fHeader._firstFreePageID == 0) // no free page, need allocate a new page
    {
        // printf("MOM\n");
        // update meta page
        _fHeader._pageNum++; // allocate a new page
        _fHeader._firstFreePageID = _fHeader._pageNum - 1;
        _fHeaderChange = true; // flag for writing back
        // get new page
        int index;
        BufType pageBuf = _bpm->getPage(_fileID, _fHeader._firstFreePageID, index);
        memset(pageBuf, 0, PAGE_SIZE); // NEXT_FREE_PAGE_ID = 0 (META_PAGE_ID), it means no more free page
        _bpm->markDirty(index);
        _bpm->writeBack(index);
    }
    // printf("WOW\n");
    int index;
    BufType pageBuf = _bpm->getPage(_fileID, _fHeader._firstFreePageID, index);
    // printf("FREE PG ID %d\n", pageBuf[NEXT_FREE_PAGE_ID]);
    Byte *bitMap = (Byte *)pageBuf + PAGE_SIZE - 1;
    if (!_getFirstZeroBit(bitMap, rid._slotID))
    {
        _bpm->access(index);
        return false;
    }
    rid._pageID = _fHeader._firstFreePageID;
    _setBitOne(bitMap, rid._slotID);
    Byte *dataDst = (Byte *)(pageBuf) + PAGE_HEADER_LEN + rid._slotID * _fHeader._slotLen;
    memcpy(dataDst, dataSrc, _fHeader._slotLen);
    if (++pageBuf[USED_SLOT_NUM] == _fHeader._maxSlotNum) // free -> full used
    {
        // update meta page
        _fHeader._firstFreePageID = pageBuf[NEXT_FREE_PAGE_ID];
        // printf("WOW, FULL USED, NEXT FREE PG ID%d is %d!\n", _fHeader._firstFreePageID);
        _fHeaderChange = true; // flag for writing back
    }
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    return true;
}

bool RecordHandler::deleteRec(const RID &rid)
{
    int index;
    BufType pageBuf = _bpm->getPage(_fileID, rid._pageID, index);
    Byte *bitMap = (Byte *)pageBuf + PAGE_SIZE - 1;
    if (!_getBit(bitMap, rid._slotID))
    {
        _bpm->access(index);
        printf("No matched RID you wanna delete in this table.\n");
        return false;
    }
    _setBitZero(bitMap, rid._slotID);
    _bpm->markDirty(index);
    // _bpm->writeBack(index);
    if (pageBuf[USED_SLOT_NUM]-- == _fHeader._maxSlotNum) // a new free page
    {
        // insert this page to the linked list as the first
        pageBuf[NEXT_FREE_PAGE_ID] = _fHeader._firstFreePageID;
        // update meta page
        _fHeader._firstFreePageID = rid._pageID;
        _fHeaderChange = true; // flag for writing back
    }
    printf("Delete in rec well.\n");
    return true;
}

bool RecordHandler::updateRec(const RID &rid, const Byte *dataSrc) const
{
    int index;
    BufType pageBuf = _bpm->getPage(_fileID, rid._pageID, index);
    Byte *bitMap = (Byte *)pageBuf + PAGE_SIZE - 1;
    if (!_getBit(bitMap, rid._slotID))
    {
        _bpm->access(index);
        printf("No matched RID you wanna update in this table.\n");
        return false;
    }
    Byte *dataDst = (Byte *)pageBuf + PAGE_HEADER_LEN + rid._slotID * _fHeader._slotLen;
    memcpy(dataDst, dataSrc, _fHeader._slotLen);
    _bpm->markDirty(index);
    // _bpm->writeBack(index);
    printf("Update well.\n");
    return true;
}
