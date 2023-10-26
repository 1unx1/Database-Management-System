#pragma once

#include "../defines.h"
#include "RecordHandler.h"

class RecordScanner
{
private:
    RecordHandler *_recHandler;
    // start finding at (_startPageID, _startSlotID)
    int _startPageID;
    int _startSlotID;
    Byte *_recData;

public:
    void openScan(RecordHandler *recHandler);

    bool getNextRecord(RID &rid, Byte *data);

    bool closeScan() const;
};
