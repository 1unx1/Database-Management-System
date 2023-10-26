#include "RecordManager.h"

bool RecordManager::createRecFile(const char *fileName, int recLen, int attrNum) const
{
    // create and open new file for record
    if (!_fm->createFile(fileName))
        return false;
    int fileID;
    if (!_fm->openFile(fileName, fileID))
        return false;
    // write FileHeader on page 0
    // 8 Bytes for page header: FREE_SLOT_NUM & NEXT_FREE_PAGE_ID
    // Page Structrue: [ usedSlotNum | nextFreePageID | slots | existBitMap ]
    RM_FileHeader fHeader;
    // slot: [ nullBitMap(nullLen) | column 1 | column 2 | ... | column n ]
    fHeader._nullLen = (attrNum + 7) / 8;
    fHeader._slotLen = recLen + fHeader._nullLen;
    // 1 bit for existBitMap
    fHeader._maxSlotNum = (PAGE_SIZE - PAGE_HEADER_LEN) * 8 / (fHeader._slotLen * 8 + 1);
    fHeader._attrNum = attrNum;
    int index;
    BufType pageBuf = _bpm->getPage(fileID, META_PAGE_ID, index);
    memset(pageBuf, 0, PAGE_SIZE);
    memcpy(pageBuf, &fHeader, sizeof(RM_FileHeader));
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    // close file
    _fm->closeFile(fileID);
    return true;
}

bool RecordManager::deleteRecFile(const char *fileName) const
{
    _bpm->close();
    std::string command = "rm " + std::string(fileName);
    system(command.c_str());
    return true;
}

bool RecordManager::openRecFile(const char *fileName, RecordHandler &recHandler) const
{
    // get fileID
    int fileID;
    if (!_fm->openFile(fileName, fileID))
        return false;
    // initialize handler
    recHandler.init(fileID, _bpm, _fm);
    return true;
}

bool RecordManager::closeRecFile(RecordHandler &recHandler) const
{
    if (recHandler._fHeaderChange) // page 0 which stored FileHeader need write back
    {
        int index;
        BufType pageBuf = _bpm->getPage(recHandler._fileID, META_PAGE_ID, index);
        memcpy(pageBuf, &recHandler._fHeader, sizeof(RM_FileHeader));
        _bpm->markDirty(index);
        _bpm->writeBack(index);
    }
    _bpm->close(); // write back all pages which need to be write back
    // printf("close recfile.\n");
    _fm->closeFile(recHandler._fileID);
    return true;
}