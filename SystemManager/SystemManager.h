#pragma once

#include <ctime>
#include "../defines.h"
#include "../RecordManager/RecordManager.h"
#include "../RecordManager/RecordScanner.h"
#include "../IndexManager/IndexManager.h"
#include "../IndexManager/IndexScanner.h"
#include "../FileSystem/bufmanager/BufPageManager.h"
#include "../FileSystem/fileio/FileManager.h"

struct Expr
{
    Expr *_l;
    Expr *_r;
    CmpOp _op; // when Expr is used for setting, op 'EQ' means assigning
    bool _isVal;
    bool _isColumn;
    // isVal
    AttrVal _val;
    // isColumn
    std::string _tbName;
    std::string _colName;

    // isOp, like A.a op val, A.a op B.b
    Expr(Expr *l, Expr *r, CmpOp op)
        : _l(l), _r(r), _op(op), _isVal(false), _isColumn(false) {}

    // isVal
    Expr(int intVal)
        : _l(nullptr), _r(nullptr), _op(NONE), _isVal(true), _isColumn(false)
    {
        _val._type = INT;
        _val._intVal = intVal;
    }

    Expr(float floatVal)
        : _l(nullptr), _r(nullptr), _op(NONE), _isVal(true), _isColumn(false)
    {
        _val._type = FLOAT;
        _val._floatVal = floatVal;
    }

    Expr(const char *charVal, int len)
        : _l(nullptr), _r(nullptr), _op(NONE), _isVal(true), _isColumn(false)
    {
        _val._type = VARCHAR;
        _val._setVarcharVal(charVal, len);
    }

    Expr(bool isNull)
        : _l(nullptr), _r(nullptr), _op(NONE), _isVal(true), _isColumn(false)
    {
        _val._isNull = true;
    }

    // isColumn
    Expr(const char *columnName, const char *tableName)
        : _l(nullptr), _r(nullptr), _op(NONE), _isVal(false), _isColumn(true)
    {
        if (tableName != nullptr)
            _tbName = std::string(tableName);
        _colName = std::string(columnName);
    }
};

class SystemManager
{
public:
    BufPageManager *_bpm;
    FileManager *_fm;
    RecordManager *_rm;
    IndexManager *_im;
    SystemMeta _sysMeta;
    int _sysFileID;
    bool _usingDB;
    DBMeta _DBMeta;      // meta info of DB in use
    std::string _DBName; // name of DB in use

    // expr like A.a op val, A.a op A.b
    bool _satisfy(const Expr &condExpr, int tbID, const int *offsets, const Byte *recData) const;

    // expr like A.a op B.b
    bool _satisfy(const Expr &condExpr, int lTbID, int rTbID, const int *lOffsets, const int *rOffsets,
                  const Byte *lRecData, const Byte *rRecData) const;

    int _getTableID(const char *tableName) const;

    // must make sure this table exists
    int _getColumnID(int tbID, const char *columnName) const;

    bool _loadDBMeta(const char *DBName);

    void _writeBackSysMeta() const;

    void _writeBackDBMeta() const;

    // for QM
    bool _foundInPrimaryKey(const Byte *cmpKey, int priTbID) const;

    bool _foundInForeignKey(const Byte *cmpKey, const char *forTbName, int forTbID, int forColumnID) const;

    // must make sure this table exists
    bool _checkConstraintWhenInsert(const Byte *recData, int tbID) const;

    // must make sure this table exists
    bool _checkConstraintWhenDelete(const char *tableName, int tbID, const Byte *recData, bool callByUpdate = false) const;

    // must make sure this table exists
    bool _checkConstraintWhenUpdate(const char *tableName, int tbID, const Byte *oldRecData, const Byte *newRecData, std::vector<int> *columnIDs) const;

    // must make sure this table exists
    int _getRecLen(int tbID) const;

    // must make sure this table exists
    bool _setRecData(Byte *recData, int tbID, const std::vector<Expr> *valList, bool safe = false) const;

    // keep opening system.meta
    SystemManager(BufPageManager *bpm, FileManager *fm, RecordManager *rm, IndexManager *im)
        : _bpm(bpm), _fm(fm), _rm(rm), _im(im), _usingDB(false)
    {
        if (!_fm->openFile(sysMetaFileName, _sysFileID))
        {
            _fm->createFile(sysMetaFileName);
            _fm->openFile(sysMetaFileName, _sysFileID);
        }
        int index;
        BufType pageBuf = _bpm->getPage(_sysFileID, 0, index);
        memcpy(&_sysMeta, pageBuf, sizeof(SystemMeta));
        _bpm->access(index);
    }

    ~SystemManager()
    {
        _writeBackSysMeta();
    }

    // system
    void showDBs() const;

    // DB
    bool createDB(const char *DBName);

    bool dropDB(const char *DBName);

    bool useDB(const char *DBName);

    void showUse() const;

    bool showTables() const;

    // Table
    bool createTable(const char *tableName, const std::vector<Column> *columnList);

    bool dropTable(const char *tableName, bool callByDropTable = false);

    bool descTable(const char *tableName) const;

    // Index
    bool addIndex(const char *tableName, const char *attrName);

    bool dropIndex(const char *tableName, const char *attrName);

    // Key
    bool addPrimaryKey(const char *tableName,
                       const char *attrName,
                       bool callByCreateTb = false);

    bool dropPrimaryKey(const char *tableName);

    bool addForeignKey(const char *name,
                       const char *forTbName, const char *forAttr,
                       const char *priTbName, const char *priAttr,
                       bool callByCreateTb = false);

    bool dropForeignKey(const char *tableName, const char *forKeyName);
};
