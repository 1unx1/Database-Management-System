#pragma once

#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>

typedef unsigned char Byte;
typedef char ColumnType;
// IM
#define NULL_NODE -1
// RM
#define PAGE_HEADER_LEN 8
#define META_PAGE_ID 0
#define USED_SLOT_NUM 0
#define NEXT_FREE_PAGE_ID 1
// SM
#define MAX_NAME_LEN 16
#define MAX_TABLE_NUM 10
#define MAX_REFER_NUM 4
#define MAX_FOREIGN_KEY_NUM 4
#define MAX_INDEX_NUM 10
#define MAX_COL_NUM 13
#define MAX_DB_NUM 10

// public
// enum AttrType
// {
//     INT,
//     FLOAT,
//     VARCHAR
// };

const ColumnType INT = 'I';
const ColumnType FLOAT = 'F';
const ColumnType VARCHAR = 'V';

struct AttrVal
{
    ColumnType _type = INT;
    bool _isNull = false;
    int _intVal = 0;
    float _floatVal = 0;
    char *_varcharVal = nullptr;
    int _varcharLen = 0;

    void _setVarcharVal(const char *varchar, int len);

    const Byte *_val() const;

    int _len() const;
};

struct NullUtil
{
    static bool isNull(const Byte *recData, int columnID)
    {
        return (recData[columnID / 8] >> (columnID % 8)) & 0x1;
    }

    static void setNullBit(Byte *recData, int columnID, bool isNull)
    {
        if (isNull)
            recData[columnID / 8] |= (0x1 << (columnID % 8));
        else
            recData[columnID / 8] &= (~(0x1 << (columnID % 8)));
    }
};

const char sysMetaFileName[] = "system.meta";

struct NameUtil
{
    static void cpy(char *dst, const char *src)
    {
        strncpy(dst, src, MAX_NAME_LEN);
    }

    static int cmp(const char *name1, const char *name2)
    {
        return strncmp(name1, name2, MAX_NAME_LEN);
    }

    static std::string DBMetaFileName(const char *DBName)
    {
        return std::string(DBName) + ".dbmeta";
    }

    static std::string recFileName(const char *DBName, const char *tableName)
    {
        return std::string(DBName) + "_" + std::string(tableName) + ".rec";
    }

    static std::string indexFileName(const char *fileName, int idxOfIndex)
    {
        return std::string(fileName) + "_" + std::to_string(idxOfIndex) + ".idx";
    }
};

enum CmpOp
{
    LT,
    LE,
    GT,
    GE,
    EQ,
    NE,
    NONE
};

class CmpUtil
{
private:
    static float _compare(ColumnType type, const Byte *data1, const Byte *data2, int len)
    {
        float cmp;
        switch (type)
        {
        case INT:
            cmp = *((int *)data1) - *((int *)data2);
            break;
        case FLOAT:
            cmp = *((float *)data1) - *((float *)data2);
            break;
        case VARCHAR:
            cmp = strncmp((const char *)data1, (const char *)data2, len);
            break;
        default:
            break;
        }
        return cmp;
    }

public:
    // check if data1 op data2
    static bool match(ColumnType type, const Byte *data1, CmpOp op, const Byte *data2, int len)
    {
        // if (type == INT)
        //     printf("if %d op(%d) %d ?\n", *((int *)data1), op, *((int *)data2));
        // else if (type == FLOAT)
        //     printf("if %f op(%d) %f ?\n", *((float *)data1), op, *((float *)data2));
        // if (type == VARCHAR)
        //     printf("if %s op(%d) %s ?\n", data1, op, data2);
        switch (op)
        {
        case LT:
            return _compare(type, data1, data2, len) < 0;
        case LE:
            return _compare(type, data1, data2, len) <= 0;
        case GT:
            return _compare(type, data1, data2, len) > 0;
        case GE:
            return _compare(type, data1, data2, len) >= 0;
        case EQ:
            return _compare(type, data1, data2, len) == 0;
        case NE:
            return _compare(type, data1, data2, len) != 0;
        default:
            break;
        }
        return false;
    }
};

// RM
struct RID
{
    int _pageID;
    int _slotID;
    RID(int pageID, int slotID)
        : _pageID(pageID), _slotID(slotID) {}
};

struct RM_FileHeader
{
    int _pageNum = 1;         // 0: meta page, 1~n: data page, contains meta page
    int _firstFreePageID = 0; // the first of linked list of free pages
    int _nullLen;
    int _slotLen;
    int _maxSlotNum;
    int _attrNum;
};

// IM
struct PageHeader
{
    bool _isLeaf;
    int _prevPageID;
    int _nextPageID;
    int _dadPageID;
    int _keyNum;
};

struct IDX_FileHeader
{
    ColumnType _keyType;
    int _keyLen;
    int _rootPageID; // the root of B+ tree
    int _nodeNum;    // the number of nodes in B+ tree
    int _maxKeyNum;  // the order of B+ tree
    int _nextFreePageID;
    // the offset from the start of the page of 4 fields
    int _keyOffset;
    int _pageIDOffset;
    int _slotIDOffset;
    int _childIDOffset;
};

struct BTreeNode
{
    PageHeader _pHeader;
    Byte *pageBuf;
    // entry = (key, pageID, slotID, childID)
    Byte *_keys;
    int *_pageIDs;
    int *_slotIDs;
    int *_childIDs;
    // Page Structrue: [ isLeaf | prevPageID | mextPageID | dadPageID | keyNum | keys | pageIDs | slotIDs | childIDs ]
    int index; // not stored in page
};

// SM
struct Column
{
    char _name[MAX_NAME_LEN]{};
    ColumnType _attrType;
    int _attrLen;
    bool _canBeNull = true;
    // bool _hasDefault = false;
    // AttrVal *_defaultVal;
    bool _isPriKey = false;
    bool _hasForKey = false;
};

struct Index
{
    bool _exist = false;
    bool _ofPK = false;
    int _columnID; // index in TableMeta._columns
};

struct FK
{
    char _forColName[MAX_NAME_LEN]{};
    char _priTbName[MAX_NAME_LEN]{};
    char _priColName[MAX_NAME_LEN]{};
};

struct ForeignKey
{
    char _name[MAX_NAME_LEN]{};
    int _priTbID = -1;
    int _priColumnID;
    int _forTbID;
    int _forColumnID;
};

struct Reference
{
    int _priTbID = -1;
    int _priColumnID;
    int _forTbID;
    int _forColumnID;
};

struct PK
{
    char _columnName[MAX_NAME_LEN]{};
};

struct PrimaryKey
{
    bool _exist = false;
    int _columnID; // index in TableMeta._columns
    int _indexID;  // index in TableMeta._indexes
    Reference _refers[MAX_REFER_NUM];
};

struct TableMeta
{
    char _name[MAX_NAME_LEN]{};
    int _columnNum;
    Column _columns[MAX_COL_NUM];
    Index _indexes[MAX_INDEX_NUM];
    PrimaryKey _primaryKey;
    ForeignKey _foreignKeys[MAX_FOREIGN_KEY_NUM];

    inline int nullByteLen() const
    {
        return (_columnNum + 7) / 8;
    }
};

struct SystemMeta
{
    char _DBNames[MAX_DB_NUM][MAX_NAME_LEN]{};
};

struct DBMeta
{
    char _tbNames[MAX_TABLE_NUM][MAX_NAME_LEN]{};
    TableMeta _tbMetas[MAX_TABLE_NUM];
};