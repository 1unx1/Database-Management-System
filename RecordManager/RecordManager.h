#pragma once

#include "../defines.h"
#include "RecordHandler.h"

class RecordManager
{
private:
    BufPageManager *_bpm;
    FileManager *_fm;

public:
    RecordManager(BufPageManager *bpm, FileManager *fm) : _bpm(bpm), _fm(fm) {}

    bool createRecFile(const char *fileName, int recLen, int attrNum) const;

    bool deleteRecFile(const char *fileName) const;

    bool openRecFile(const char *fileName, RecordHandler &recHandler) const;

    bool closeRecFile(RecordHandler &recHandler) const;
};