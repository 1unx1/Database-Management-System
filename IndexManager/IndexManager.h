#pragma once

#include "../defines.h"
#include "../FileSystem/bufmanager/BufPageManager.h"
#include "../FileSystem/fileio/FileManager.h"
#include "IndexHandler.h"

class IndexManager
{
private:
    BufPageManager *_bpm;
    FileManager *_fm;

public:
    IndexManager(BufPageManager *bpm, FileManager *fm) : _bpm(bpm), _fm(fm) {}

    bool createIndexFile(const char *fileName, int idxOfIndex, ColumnType attrType, int attrLen) const;

    bool destroyIndexFile(const char *fileName, int idxOfIndex) const;

    bool openIndexFile(const char *fileName, int idxOfIndex, IndexHandler &idxHandler) const;

    bool closeIndexFile(IndexHandler &idxHandler) const;
};
