#pragma once

#include "../defines.h"
#include "../FileSystem/bufmanager/BufPageManager.h"
#include "../FileSystem/fileio/FileManager.h"
#include "../RecordManager/RecordHandler.h"

// keep page 0 stay in buffer, write it back when manager close file

class IndexHandler
{
private:
    bool _compare(CmpOp op, const Byte *key1, int pageID1, int slotID1,
                  const Byte *key2, int pageID2, int slotID2) const;

    BTreeNode *_loadNode(int nodePageID) const;

    void _setEntry(BTreeNode *node, int idx, const Byte *key, int pageID, int slotID, int childID = NULL_NODE) const;

    void _writeNodeBack(BTreeNode *node, int nodePageID) const;

    void _moveEntries(BTreeNode *dstNode, int dstIdx, BTreeNode *srcNode, int srcIdx, int n) const;

    inline bool _overflow(int keyNum) const;

    inline bool _underflow(int keyNum) const;

    void _average(BTreeNode *leftNode, int leftNodePageID, BTreeNode *rightNode, int rightNodePageID) const;

    void _handleOverflow(BTreeNode *node, int nodePageID);

    void _handleUnderflow(BTreeNode *node, int nodePageID);

    void _insertEntryInNode(BTreeNode *node, int nodePageID, int idx, int pageID, int slotID, const Byte *key, BTreeNode *childNode, int childID);

    void _deleteEntryInNode(BTreeNode *node, int nodePageID, int idx);

public:
    int _fileID;
    BufPageManager *_bpm;
    FileManager *_fm;
    IDX_FileHeader _fHeader;
    bool _fHeaderChange;

    // for Scanner
    bool _getFirstEntry(int &targetPageID, int &idx, Byte *firstKey) const;

    bool _getNextEntry(int &targetPageID, int &idx, Byte *nextKey) const;

    void _getRID(int nodePageID, int idx, RID &rid) const;

    void init(int fileID, BufPageManager *bpm, FileManager *fm);

    // when insert == false, find the max entry <= (key, rid), may be not existing (idx = -1)
    // when insert == true, find which idx (key, rid) should be
    void findEntry(const RID &rid, const Byte *key, bool insert, int &idx, int &targetPageID, bool &last, Byte *getKey = nullptr) const;

    bool insertEntry(const RID &rid, const Byte *key);

    bool deleteEntry(const RID &rid, const Byte *key);
};
