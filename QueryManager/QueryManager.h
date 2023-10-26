#pragma once

#include <ctime>
#include "../defines.h"
#include "../SystemManager/SystemManager.h"

class QueryManager
{
private:
    SystemManager *_sm;

    // expr like A.a = val
    bool _checkSetList(const char *tableName, int tbID, const std::vector<Expr> *setList,
                       std::vector<int> *columnIDs) const;

    // expr like A.a
    bool _checkColumnList(const char *tableName, int tbID, const std::vector<Expr> *columnList,
                          std::vector<int> *columnIDs) const;

    // expr like A.a, B.b
    bool _checkColumnListWhenJoin(const char *tableNameA, const char *tableNameB,
                                  int tbAID, int tbBID, const std::vector<Expr> *columnList) const;

    // expr like A.a op val, A.a op A.b
    bool _checkCondList(const char *tableName, int tbID, const std::vector<Expr> *condList) const;

    // expr like A.a op B.b, B.b op A.a, A.a op A.b, B.a op B.b, A.a op val, B.b op val
    bool _checkCondListWhenJoin(const char *tableNameA, const char *tableNameB,
                                int tbAID, int tbBID, const std::vector<Expr> *condList) const;

    // deal with cond expr like A.a op val, A.a op A.b
    // if callByJoin, it means condList may contain B.b op val, B.a op B.b, B.a op A.a
    std::vector<RID> _filter(const char *tableName, int tbID,
                             const std::vector<Expr> *condList, bool callByJoin = false) const;

    // cond expr expr like A.a op B.b, B.b op A.a, A.a op A.b, B.a op B.b, A.a op val, B.b op val
    // column expr like A.a
    // outer loop A, inner loop B
    std::vector<Byte *> *_join(const char *tableNameA, const char *tableNameB, int tbAID, int tbBID,
                               const std::vector<Expr> *columnList, const std::vector<Expr> *condList,
                               const std::vector<Byte *> *recDataAs, TableMeta &resultTbMeta) const;

    int *_getOffsets(const TableMeta &tbMeta) const;

    void _showRec(const TableMeta &tbMeta, const int *offsets, const Byte *recData,
                  const std::vector<int> *columnIDs) const;

public:
    QueryManager(SystemManager *sm) : _sm(sm) {}

    bool insertRec(const char *tableName, const std::vector<Expr> *valList, bool safe = false) const;

    bool safeInsert(const char *tableName, const std::vector<std::vector<Expr>> *valLists) const;

    bool deleteRec(const char *tableName, const std::vector<Expr> *condList) const;

    bool updateRec(const char *tableName,
                   const std::vector<Expr> *setList,
                   const std::vector<Expr> *condList) const;

    bool selectRec(bool join, const char *tableNameA, const char *tableNameB,
                   const std::vector<Expr> *columnList,
                   std::vector<Expr> *condList) const;
};
