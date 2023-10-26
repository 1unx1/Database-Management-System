#include "RecordScanner.h"

void RecordScanner::openScan(RecordHandler *recHandler)
{
    // initialization
    _recHandler = recHandler;
    _startPageID = 1; // page 0 is meta page
    _startSlotID = 0;
    _recData = new Byte[_recHandler->_fHeader._slotLen];
}

bool RecordScanner::getNextRecord(RID &rid, Byte *data)
{
    RID tempRID(0, 0);
    if (!_recHandler->_getNextRec(_startPageID, _startSlotID, tempRID, _recData))
        return false;
    // update start location, from the next slot after rid, whether equal or not
    _startPageID = tempRID._pageID;
    _startSlotID = tempRID._slotID + 1;
    rid = tempRID;
    memcpy(data, _recData, _recHandler->_fHeader._slotLen);
    return true;
}

bool RecordScanner::closeScan() const
{
    delete[] _recData;
    return true;
}
