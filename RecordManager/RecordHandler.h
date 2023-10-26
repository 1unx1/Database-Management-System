#pragma once

#include "../defines.h"
#include "../FileSystem/bufmanager/BufPageManager.h"
#include "../FileSystem/fileio/FileManager.h"

// Page Structrue: [ usedSlotNum | nextFreePageID | slots | n nullBitMaps | existBitMap ]
// slot: [ nullBitMap(nullLen) | column 1 | column 2 | ... | column n ]
// keep page 0 stay in buffer, write it back when manager close file

class RecordHandler
{
private:
    inline bool _getBit(Byte *bitMap, int idx) const;

    inline void _setBitOne(Byte *bitMap, int idx) const;

    inline void _setBitZero(Byte *bitMap, int idx) const;

    bool _getFirstZeroBit(Byte *bitMap, int &idx) const;

    bool _getNextOneBit(Byte *bitMap, int startIdx, int &idx) const;

public:
    int _fileID;
    BufPageManager *_bpm;
    FileManager *_fm;
    RM_FileHeader _fHeader;
    bool _fHeaderChange;

    // for Scanner
    bool _getNextRec(int startPageID, int startSlotID, RID &rid, Byte *dataDst) const;

    bool _getRecNum(int pageID, int &recNum) const;

    void init(int fileID, BufPageManager *bpm, FileManager *fm);

    bool accessRec(const RID &rid, Byte *dataDst) const;

    bool insertRec(const Byte *dataSrc, RID &rid);

    bool deleteRec(const RID &rid);

    bool updateRec(const RID &rid, const Byte *dataSrc) const;
};
