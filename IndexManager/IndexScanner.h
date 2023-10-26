#pragma once

#include "../defines.h"
#include "../RecordManager/RecordHandler.h"
#include "IndexHandler.h"

class IndexScanner
{
private:
    IndexHandler *_idxHandler;
    bool _firstTime;
    bool _noMore; // no more matched entry
    Byte *_cmpKey;
    // leafNode's _idx key is _findKey (when _idx is legal)
    int _leafPageID;
    int _idx;
    Byte *_findKey;
    CmpOp _op;

public:
    // to find key op cmpKey is true
    void openScan(IndexHandler *idxHandler, const Byte *cmpKey, CmpOp op);

    bool getNextEntry(RID &rid);

    bool closeScan() const;
};
