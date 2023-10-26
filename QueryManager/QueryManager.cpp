#include "QueryManager.h"

// expr like A.a = val
bool QueryManager::_checkSetList(const char *tableName, int tbID, const std::vector<Expr> *setList,
                                 std::vector<int> *columnIDs) const
{
    for (const Expr &setExpr : *setList) // expr like A.a = val
    {
        if (setExpr._l == nullptr || setExpr._r == nullptr)
        {
            printf("Not a set expr.\n");
            return false;
        }
        if (!setExpr._l->_isColumn || !setExpr._r->_isVal)
        {
            printf("Wrong format of set expr.\n");
            return false;
        }
        if (setExpr._l->_tbName.length() != 0 && NameUtil::cmp(setExpr._l->_tbName.c_str(), tableName) != 0)
        {
            printf("Wrong table name in set list.\n");
            return false;
        }
        int columnID = _sm->_getColumnID(tbID, setExpr._l->_colName.c_str());
        if (columnID < 0) // column not existing
        {
            printf("Wrong column name in set list.\n");
            return false;
        }
        if (setExpr._r->_val._type != _sm->_DBMeta._tbMetas[tbID]._columns[columnID]._attrType)
        {
            printf("Type mismatch in set list.\n");
            return false;
        }
        columnIDs->push_back(columnID);
    }
    return true;
}

// expr like A.a
bool QueryManager::_checkColumnList(const char *tableName, int tbID, const std::vector<Expr> *columnList,
                                    std::vector<int> *columnIDs) const
{
    if (columnList == nullptr) // select *
        return true;
    for (const Expr &columnExpr : *columnList) // expr like A.a
    {
        if (columnExpr._l != nullptr || columnExpr._r != nullptr)
        {
            printf("Not a column expr.\n");
            return false;
        }
        if (columnExpr._tbName.length() != 0 && NameUtil::cmp(columnExpr._tbName.c_str(), tableName) != 0)
        {
            printf("Wrong table name in set list.\n");
            return false;
        }
        int columnID = _sm->_getColumnID(tbID, columnExpr._colName.c_str());
        if (columnID < 0) // column not existing
        {
            printf("Non-existing column.\n");
            return false;
        }
        columnIDs->push_back(columnID);
    }
    return true;
}

// expr like A.a, B.b
bool QueryManager::_checkColumnListWhenJoin(const char *tableNameA, const char *tableNameB,
                                            int tbAID, int tbBID, const std::vector<Expr> *columnList) const
{
    if (columnList == nullptr) // select *
        return true;
    for (const Expr &columnExpr : *columnList) // expr like A.a, B.b
    {
        if (columnExpr._l != nullptr || columnExpr._r != nullptr)
        {
            printf("Not a column expr.\n");
            return false;
        }
        if (columnExpr._tbName.length() != 0 &&
            NameUtil::cmp(columnExpr._tbName.c_str(), tableNameA) != 0 &&
            NameUtil::cmp(columnExpr._tbName.c_str(), tableNameB) != 0)
        {
            printf("Wrong table name in column list.\n");
            return false;
        }
        if (NameUtil::cmp(columnExpr._tbName.c_str(), tableNameA) == 0)
        {
            if (_sm->_getColumnID(tbAID, columnExpr._colName.c_str()) < 0)
            {
                printf("Wrong column name in column list.\n");
                return false;
            }
        }
        else
        {
            if (_sm->_getColumnID(tbBID, columnExpr._colName.c_str()) < 0)
            {
                printf("Wrong column name in column list.\n");
                return false;
            }
        }
    }
    return true;
}

// expr like A.a op val, A.a op A.b
bool QueryManager::_checkCondList(const char *tableName, int tbID, const std::vector<Expr> *condList) const
{
    if (condList == nullptr)
        return true;
    const TableMeta &tbMeta = _sm->_DBMeta._tbMetas[tbID];
    for (const Expr &condExpr : *condList)
    {
        if (condExpr._l == nullptr || condExpr._r == nullptr)
        {
            printf("Not a cond expr.\n");
            return false;
        }
        if (condExpr._l->_isVal)
        {
            printf("Wrong format of cond expr.\n");
            return false;
        }
        if (condExpr._l->_tbName.length() != 0 && NameUtil::cmp(condExpr._l->_tbName.c_str(), tableName) != 0)
        {
            printf("Wrong table name in cond list.\n");
            return false;
        }
        int lColumnID = _sm->_getColumnID(tbID, condExpr._l->_colName.c_str());
        if (lColumnID < 0)
        {
            for (int i = 0; i <= _sm->_DBMeta._tbMetas[tbID]._columnNum - 1; i++)
                printf("%s, ", _sm->_DBMeta._tbMetas[tbID]._columns[i]._name);
            printf("Wrong column name %s in cond list.", condExpr._l->_colName.c_str());
            return false;
        }
        if (condExpr._r->_isVal)
        {
            if (condExpr._r->_val._type != tbMeta._columns[lColumnID]._attrType)
            {
                printf("Type mismatch.\n");
                return false;
            }
        }
        else
        {
            if (condExpr._r->_tbName.length() != 0 && NameUtil::cmp(condExpr._r->_tbName.c_str(), tableName) != 0)
            {
                printf("Wrong table name in cond list.\n");
                return false;
            }
            int rColumnID = _sm->_getColumnID(tbID, condExpr._r->_colName.c_str());
            if (rColumnID < 0)
            {
                printf("Wrong column name in cond list.\n");
                return false;
            }
            if (tbMeta._columns[lColumnID]._attrType != tbMeta._columns[rColumnID]._attrType ||
                tbMeta._columns[lColumnID]._attrLen != tbMeta._columns[rColumnID]._attrLen)
            {
                printf("Type mismatch.\n");
                return false;
            }
        }
    }
    // printf("Nice cond list.\n");
    return true;
}

// expr like A.a op B.b, B.b op A.a, A.a op A.b, B.a op B.b, A.a op val, B.b op val
bool QueryManager::_checkCondListWhenJoin(const char *tableNameA, const char *tableNameB,
                                          int tbAID, int tbBID, const std::vector<Expr> *condList) const
{
    if (condList == nullptr)
        return true;
    const TableMeta &tbAMeta = _sm->_DBMeta._tbMetas[tbAID];
    const TableMeta &tbBMeta = _sm->_DBMeta._tbMetas[tbBID];
    for (const Expr &condExpr : *condList)
    {
        if (condExpr._l == nullptr || condExpr._r == nullptr)
        {
            printf("Not a cond expr.\n");
            return false;
        }
        if (condExpr._l->_isVal)
        {
            printf("Wrong format of cond expr.\n");
            return false;
        }
        if (condExpr._r->_isVal) // A.a op val, B.b op val
        {
            if (condExpr._l->_tbName.length() != 0 &&
                NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameA) != 0 &&
                NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) != 0)
            {
                printf("Wrong table name in cond list.\n");
                return false;
            }
            if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameA) == 0)
            {
                int lColumnID = _sm->_getColumnID(tbAID, condExpr._l->_colName.c_str());
                if (lColumnID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                if (condExpr._r->_val._type != tbAMeta._columns[lColumnID]._attrType)
                {
                    printf("Type mismatch.\n");
                    return false;
                }
            }
            else
            {
                int lColumnID = _sm->_getColumnID(tbBID, condExpr._l->_colName.c_str());
                if (lColumnID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                if (condExpr._r->_val._type != tbBMeta._columns[lColumnID]._attrType)
                {
                    printf("Type mismatch.\n");
                    return false;
                }
            }
        }
        else // A.a op B.b, B.b op A.a, A.a op A.b, B.a op B.b
        {
            if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameA) == 0 &&
                NameUtil::cmp(condExpr._r->_tbName.c_str(), tableNameB) == 0)
            {
                int columnAID = _sm->_getColumnID(tbAID, condExpr._l->_colName.c_str());
                if (columnAID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                int columnBID = _sm->_getColumnID(tbBID, condExpr._r->_colName.c_str());
                if (columnBID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                if (tbAMeta._columns[columnAID]._attrType != tbBMeta._columns[columnBID]._attrType ||
                    tbAMeta._columns[columnAID]._attrLen != tbBMeta._columns[columnBID]._attrLen)
                {
                    printf("Type mismatch.\n");
                    return false;
                }
            }
            else if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) == 0 &&
                     NameUtil::cmp(condExpr._r->_tbName.c_str(), tableNameA) == 0)
            {
                int columnBID = _sm->_getColumnID(tbBID, condExpr._l->_colName.c_str());
                if (columnBID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                int columnAID = _sm->_getColumnID(tbAID, condExpr._r->_colName.c_str());
                if (columnAID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                if (tbAMeta._columns[columnAID]._attrType != tbBMeta._columns[columnBID]._attrType ||
                    tbAMeta._columns[columnAID]._attrLen != tbBMeta._columns[columnBID]._attrLen)
                {
                    printf("Type mismatch.\n");
                    return false;
                }
            }
            else if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameA) == 0 &&
                     NameUtil::cmp(condExpr._r->_tbName.c_str(), tableNameA) == 0)
            {
                int lColumnID = _sm->_getColumnID(tbAID, condExpr._l->_colName.c_str());
                if (lColumnID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                int rColumnID = _sm->_getColumnID(tbAID, condExpr._r->_colName.c_str());
                if (rColumnID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                if (tbAMeta._columns[lColumnID]._attrType != tbAMeta._columns[rColumnID]._attrType ||
                    tbAMeta._columns[lColumnID]._attrLen != tbAMeta._columns[rColumnID]._attrLen)
                {
                    printf("Type mismatch.\n");
                    return false;
                }
            }
            else if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) == 0 &&
                     NameUtil::cmp(condExpr._r->_tbName.c_str(), tableNameB) == 0)
            {
                int lColumnID = _sm->_getColumnID(tbBID, condExpr._l->_colName.c_str());
                if (lColumnID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                int rColumnID = _sm->_getColumnID(tbBID, condExpr._r->_colName.c_str());
                if (rColumnID < 0)
                {
                    printf("Wrong column name in cond list.\n");
                    return false;
                }
                if (tbBMeta._columns[lColumnID]._attrType != tbBMeta._columns[rColumnID]._attrType ||
                    tbBMeta._columns[lColumnID]._attrLen != tbBMeta._columns[rColumnID]._attrLen)
                {
                    printf("Type mismatch.\n");
                    return false;
                }
            }
            else
            {
                printf("Wrong table name in cond list.\n");
                return false;
            }
        }
    }
    return true;
}

// deal with cond expr like A.a op val, A.a op A.b
// if callByJoin, it means condList may contain B.b op val, B.a op B.b, B.a op A.a
std::vector<RID> QueryManager::_filter(const char *tableName, int tbID,
                                       const std::vector<Expr> *condList, bool callByJoin) const
{
    std::vector<RID> rids{};
    const TableMeta &tbMeta = _sm->_DBMeta._tbMetas[tbID];
    int *offsets = _getOffsets(tbMeta);
    // try finding a index on A.a op val
    int indexID = -1;
    Byte *key = nullptr;
    CmpOp op;
    if (condList != nullptr)
        for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
            if (tbMeta._indexes[i]._exist)
            {
                if (!callByJoin)
                    for (const Expr &condExpr : *condList) // expr like A.a op val, A.a op A.b
                    {
                        if (condExpr._r->_isVal)
                            // A.a op val
                            if (NameUtil::cmp(condExpr._l->_colName.c_str(),
                                              tbMeta._columns[tbMeta._indexes[i]._columnID]._name) == 0)
                            // index on A.a op val
                            {
                                indexID = i;
                                key = new Byte[tbMeta._columns[tbMeta._indexes[i]._columnID]._attrLen];
                                memcpy(key, condExpr._r->_val._val(), tbMeta._columns[tbMeta._indexes[i]._columnID]._attrLen);
                                op = condExpr._op;
                                break;
                            }
                    }
                else // when join A and B, call filter to cond expr like A.a op val
                    for (const Expr &condExpr : *condList)
                        if (condExpr._r->_isVal &&
                            NameUtil::cmp(condExpr._l->_tbName.c_str(), tableName) == 0)
                            // A.a op val
                            if (NameUtil::cmp(condExpr._l->_colName.c_str(),
                                              tbMeta._columns[tbMeta._indexes[i]._columnID]._name) == 0)
                            // index on A.a op val
                            {
                                indexID = i;
                                key = new Byte[tbMeta._columns[tbMeta._indexes[i]._columnID]._attrLen];
                                memcpy(key, condExpr._r->_val._val(), tbMeta._columns[tbMeta._indexes[i]._columnID]._attrLen);
                                op = condExpr._op;
                                break;
                            }
                if (indexID != -1)
                    break;
            }
    RecordHandler recHandler;
    _sm->_rm->openRecFile(NameUtil::recFileName(_sm->_DBName.c_str(), tableName).c_str(), recHandler);
    RID rid(-1, -1);
    Byte *recData = new Byte[recHandler._fHeader._slotLen];
    if (indexID != -1) // with index
    {
        printf("Can use index for filter.\n");
        IndexHandler idxHandler;
        _sm->_im->openIndexFile(tbMeta._name, indexID, idxHandler);
        IndexScanner idxScanner;
        // printf("cmp key is %d\n", *((int *)(key)));
        idxScanner.openScan(&idxHandler, key, op);
        while (idxScanner.getNextEntry(rid))
        {
            // printf("RID (%d, %d) ", rid._pageID, rid._slotID);
            if (!recHandler.accessRec(rid, recData))
                continue;
            // printf("GET DATA: %d\n", *(int *)(recData + 1));
            // printf("Scanner get <%d>\n", *((int *)(recData + 1)));
            bool satisfy = true;
            if (!callByJoin)
                for (const Expr &condExpr : *condList) // expr like A.a op val, A.a op A.b
                {
                    if (!_sm->_satisfy(condExpr, tbID, offsets, recData))
                    {
                        satisfy = false;
                        // printf("checkover, bad.\n");
                        break;
                    }
                }
            else // when join A and B, call filter to cond expr like A.a op val, A.a op A.b
                for (const Expr &condExpr : *condList)
                    if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableName) == 0 &&
                        (condExpr._r->_isVal || NameUtil::cmp(condExpr._l->_tbName.c_str(), condExpr._r->_tbName.c_str()) == 0))
                        // A.a op val, A.a op A.b
                        if (!_sm->_satisfy(condExpr, tbID, offsets, recData))
                        {
                            satisfy = false;
                            break;
                        }
            if (satisfy)
            {
                rids.push_back(rid);
                // printf("checkover, good.\n");
            }
        }
        idxScanner.closeScan();
        _sm->_im->closeIndexFile(idxHandler);
    }
    else // no index
    {
        printf("No index for filter.\n");
        RecordScanner recScanner;
        recScanner.openScan(&recHandler);
        while (recScanner.getNextRecord(rid, recData))
        {
            bool satisfy = true;
            if (condList != nullptr)
            {
                if (!callByJoin)
                    for (const Expr &condExpr : *condList) // expr like A.a op val, A.a op A.b
                    {
                        if (!_sm->_satisfy(condExpr, tbID, offsets, recData))
                        {
                            // printf("MISMATCH!\n");
                            satisfy = false;
                            break;
                        }
                    }
                else // when join A and B, call filter to cond expr like A.a op val, A.a op A.b
                    for (const Expr &condExpr : *condList)
                        if (condExpr._r->_isVal &&
                            NameUtil::cmp(condExpr._l->_tbName.c_str(), tableName) == 0)
                            if (!_sm->_satisfy(condExpr, tbID, offsets, recData))
                            {
                                satisfy = false;
                                break;
                            }
            }
            if (satisfy)
                rids.push_back(rid);
        }
        recScanner.closeScan();
    }
    _sm->_rm->closeRecFile(recHandler);
    if (key != nullptr)
        delete[] key;
    delete[] recData;
    delete[] offsets;
    return rids;
}

// cond expr expr like A.a op B.b, B.b op A.a, A.a op A.b, B.a op B.b, A.a op val, B.b op val
// column expr like A.a
// outer loop A, inner loop B
std::vector<Byte *> *QueryManager::_join(const char *tableNameA, const char *tableNameB, int tbAID, int tbBID,
                                         const std::vector<Expr> *columnList, const std::vector<Expr> *condList,
                                         const std::vector<Byte *> *recDataAs, TableMeta &resultTbMeta) const
{
    std::vector<Byte *> *resultRecDatas = new std::vector<Byte *>;
    const TableMeta &tbBMeta = _sm->_DBMeta._tbMetas[tbBID];
    const TableMeta &tbAMeta = _sm->_DBMeta._tbMetas[tbAID];
    int *offsetAs = _getOffsets(tbAMeta);
    int *offsetBs = _getOffsets(tbBMeta);

    // for (int i = 0; i <= tbAMeta._columnNum - 1; i++)
    //     printf("%d, ", offsetAs[i]);
    // printf("\n");
    // for (int i = 0; i <= tbBMeta._columnNum - 1; i++)
    //     printf("%d, ", offsetAs[i]);
    // printf("\n");

    int recLen = ((columnList == nullptr ? (tbAMeta._columnNum + tbBMeta._columnNum) : columnList->size()) + 7) / 8;
    if (columnList != nullptr)
        for (const Expr &columnExpr : *columnList) // expr like A.a
        {
            if (NameUtil::cmp(columnExpr._tbName.c_str(), tableNameB) == 0)
                recLen += tbBMeta._columns[_sm->_getColumnID(tbBID, columnExpr._colName.c_str())]._attrLen;
            else
                recLen += tbAMeta._columns[_sm->_getColumnID(tbAID, columnExpr._colName.c_str())]._attrLen;
        }
    else // select *
    {
        for (int i = 0; i <= tbAMeta._columnNum - 1; i++)
            recLen += tbAMeta._columns[i]._attrLen;
        for (int i = 0; i <= tbBMeta._columnNum - 1; i++)
            recLen += tbBMeta._columns[i]._attrLen;
    }

    // printf("NEW REC LEN: %d\n", recLen);

    // set result tableMeta
    if (columnList != nullptr)
    {
        for (const Expr &columnExpr : *columnList) // expr like A.a
        {
            Column &column = resultTbMeta._columns[resultTbMeta._columnNum];
            if (NameUtil::cmp(columnExpr._tbName.c_str(), tableNameB) == 0)
            {
                int columnID = _sm->_getColumnID(tbBID, columnExpr._colName.c_str());
                const Column &columnB = tbBMeta._columns[columnID];
                column._attrType = columnB._attrType;
                column._attrLen = columnB._attrLen;
                if ((std::string(tableNameB) + "." + std::string(columnB._name)).length() >= MAX_NAME_LEN)
                    NameUtil::cpy(column._name, (std::string(tableNameB).substr(0, 1) + "." + std::string(columnB._name)).c_str());
                else
                    NameUtil::cpy(column._name,
                                  (std::string(tableNameB) + "." + std::string(columnB._name)).c_str());
            }
            else
            {
                int columnID = _sm->_getColumnID(tbAID, columnExpr._colName.c_str());
                const Column &columnA = tbAMeta._columns[columnID];
                column._attrType = columnA._attrType;
                column._attrLen = columnA._attrLen;
                if ((std::string(tableNameA) + "." + std::string(columnA._name)).length() >= MAX_NAME_LEN)
                    NameUtil::cpy(column._name, (std::string(tableNameA).substr(0, 1) + "." + std::string(columnA._name)).c_str());
                else
                    NameUtil::cpy(column._name,
                                  (std::string(tableNameA) + "." + std::string(columnA._name)).c_str());
            }
            resultTbMeta._columnNum++;
        }
    }
    else // select *
    {
        for (int i = 0; i <= tbAMeta._columnNum - 1; i++)
        {
            Column &column = resultTbMeta._columns[resultTbMeta._columnNum];
            const Column &columnA = tbAMeta._columns[i];
            column._attrType = columnA._attrType;
            column._attrLen = columnA._attrLen;
            // printf("Set len %d = %d\n", column._attrLen, columnA._attrLen);
            if ((std::string(tableNameA) + "." + std::string(columnA._name)).length() >= MAX_NAME_LEN)
                NameUtil::cpy(column._name, (std::string(tableNameA).substr(0, 1) + "." + std::string(columnA._name)).c_str());
            else
                NameUtil::cpy(column._name,
                              (std::string(tableNameA) + "." + std::string(columnA._name)).c_str());
            resultTbMeta._columnNum++;
            // for (int i = 0; i <= resultTbMeta._columnNum - 1; i++)
            //     printf("%d, ", resultTbMeta._columns[i]._attrLen);
            // printf("\n");
        }
        for (int i = 0; i <= tbBMeta._columnNum - 1; i++)
        {
            Column &column = resultTbMeta._columns[resultTbMeta._columnNum];
            const Column &columnB = tbBMeta._columns[i];
            column._attrType = columnB._attrType;
            column._attrLen = columnB._attrLen;
            if ((std::string(tableNameB) + "." + std::string(columnB._name)).length() >= MAX_NAME_LEN)
                NameUtil::cpy(column._name, (std::string(tableNameB).substr(0, 1) + "." + std::string(columnB._name)).c_str());
            else
                NameUtil::cpy(column._name,
                              (std::string(tableNameB) + "." + std::string(columnB._name)).c_str());
            resultTbMeta._columnNum++;
            // for (int i = 0; i <= resultTbMeta._columnNum - 1; i++)
            //     printf("%d, ", resultTbMeta._columns[i]._attrLen);
            // printf("\n");
        }
    }

    // for (int i = 0; i <= resultTbMeta._columnNum - 1; i++)
    //     printf("%d, ", resultTbMeta._columns[i]._attrLen);
    // printf("\n");

    // try finding a index
    int indexID = -1;
    // for index on B.b op val
    Byte *key = nullptr;
    CmpOp op;
    if (condList != nullptr)
        for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
            if (tbBMeta._indexes[i]._exist)
            {
                for (const Expr &condExpr : *condList)
                    if (condExpr._r->_isVal && NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) == 0)
                    // B.b op val
                    {
                        if (NameUtil::cmp(condExpr._l->_colName.c_str(), tbBMeta._columns[tbBMeta._indexes[i]._columnID]._name) == 0)
                        {
                            indexID = i;
                            key = new Byte[tbBMeta._columns[tbBMeta._indexes[i]._columnID]._attrLen];
                            memcpy(key, condExpr._r->_val._val(), tbBMeta._columns[tbBMeta._indexes[i]._columnID]._attrLen);
                            op = condExpr._op;
                            break;
                        }
                    }
                if (indexID != -1)
                    break;
            }
    RecordHandler recHandler;
    _sm->_rm->openRecFile(NameUtil::recFileName(_sm->_DBName.c_str(), tableNameB).c_str(), recHandler);
    RID rid(-1, -1);
    Byte *recDataB = new Byte[recHandler._fHeader._slotLen];
    if (indexID != -1) // with index
    {
        printf("Can use index for inner filter.\n");
        IndexHandler idxHandler;
        _sm->_im->openIndexFile(tbBMeta._name, indexID, idxHandler);
        IndexScanner idxScanner;
        idxScanner.openScan(&idxHandler, key, op);
        while (idxScanner.getNextEntry(rid))
        {
            if (!recHandler.accessRec(rid, recDataB))
                continue;
            for (const Byte *recDataA : *recDataAs)
            {
                bool satisfy = true;
                for (const Expr &condExpr : *condList)
                    if (condExpr._r->_isColumn)
                    {
                        if (NameUtil::cmp(condExpr._l->_tbName.c_str(), condExpr._r->_tbName.c_str()) != 0) // B.b op A.a, A.a op B.b
                            if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) == 0)               // B.b op A.a
                            {
                                if (!_sm->_satisfy(condExpr, tbBID, tbAID, offsetBs, offsetAs, recDataB, recDataA))
                                {
                                    satisfy = false;
                                    break;
                                }
                            }
                            else // A.a op B.b
                            {
                                if (!_sm->_satisfy(condExpr, tbAID, tbBID, offsetAs, offsetBs, recDataA, recDataB))
                                {
                                    satisfy = false;
                                    break;
                                }
                            }
                        else if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) == 0) // B.a op B.b
                        {
                            if (!_sm->_satisfy(condExpr, tbBID, offsetBs, recDataB))
                            {
                                satisfy = false;
                                break;
                            }
                        }
                        else // A.a op A.b has been used in outer loop A
                            continue;
                    }
                    else if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) == 0) // B.b op val
                    {
                        if (!_sm->_satisfy(condExpr, tbBID, offsetBs, recDataB))
                        {
                            satisfy = false;
                            break;
                        }
                    }
                    else // A.a op val has been used in outer loop A
                        continue;
                if (satisfy)
                {
                    Byte *recData = new Byte[recLen];
                    // resultRecDatas->push_back(new Byte[recLen]);
                    if (columnList != nullptr)
                    {
                        int offset = (columnList->size() + 7) / 8;
                        for (const Expr &columnExpr : *columnList) // expr like A.a
                        {
                            if (NameUtil::cmp(columnExpr._tbName.c_str(), tableNameB) == 0)
                            {
                                int columnID = _sm->_getColumnID(tbBID, columnExpr._colName.c_str());
                                const Column &columnB = tbBMeta._columns[columnID];
                                memcpy(recData + offset,
                                       recDataB + offsetBs[columnID], columnB._attrLen);
                                offset += columnB._attrLen;
                            }
                            else
                            {
                                int columnID = _sm->_getColumnID(tbAID, columnExpr._colName.c_str());
                                const Column &columnA = tbAMeta._columns[columnID];
                                memcpy(recData + offset,
                                       recDataA + offsetAs[columnID], columnA._attrLen);
                                offset += columnA._attrLen;
                            }
                        }
                    }
                    else // select *
                    {
                        int offset = (tbAMeta._columnNum + tbBMeta._columnNum + 7) / 8;
                        for (int i = 0; i <= tbAMeta._columnNum - 1; i++)
                        {
                            const Column &columnA = tbAMeta._columns[i];
                            memcpy(recData + offset,
                                   recDataA + offsetAs[i], columnA._attrLen);
                            offset += columnA._attrLen;
                        }
                        for (int i = 0; i <= tbBMeta._columnNum - 1; i++)
                        {
                            const Column &columnB = tbBMeta._columns[i];
                            memcpy(recData + offset,
                                   recDataB + offsetBs[i], columnB._attrLen);
                            offset += columnB._attrLen;
                        }
                    }
                    resultRecDatas->push_back(recData);
                }
            }
        }
        idxScanner.closeScan();
        _sm->_im->closeIndexFile(idxHandler);
    }
    else // no index
    {
        printf("No index for inner filter.\n");
        RecordScanner recScanner;
        recScanner.openScan(&recHandler);
        while (recScanner.getNextRecord(rid, recDataB))
        {
            for (const Byte *recDataA : *recDataAs)
            {
                bool satisfy = true;
                if (condList != nullptr)
                    for (const Expr &condExpr : *condList)
                        if (condExpr._r->_isColumn)
                        {
                            if (NameUtil::cmp(condExpr._l->_tbName.c_str(), condExpr._r->_tbName.c_str()) != 0) // B.b op A.a, A.a op B.b
                                if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) == 0)               // B.b op A.a
                                {
                                    if (!_sm->_satisfy(condExpr, tbBID, tbAID, offsetBs, offsetAs, recDataB, recDataA))
                                    {
                                        satisfy = false;
                                        break;
                                    }
                                }
                                else // A.a op B.b
                                {
                                    if (!_sm->_satisfy(condExpr, tbAID, tbBID, offsetAs, offsetBs, recDataA, recDataB))
                                    {
                                        satisfy = false;
                                        break;
                                    }
                                }
                            else if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) == 0) // B.a op B.b
                            {
                                if (!_sm->_satisfy(condExpr, tbBID, offsetBs, recDataB))
                                {
                                    satisfy = false;
                                    break;
                                }
                            }
                            else // A.a op A.b has been used in outer loop A
                                continue;
                        }
                        else if (NameUtil::cmp(condExpr._l->_tbName.c_str(), tableNameB) == 0) // B.b op val
                        {
                            if (!_sm->_satisfy(condExpr, tbBID, offsetBs, recDataB))
                            {
                                satisfy = false;
                                break;
                            }
                        }
                        else // A.a op val has been used in outer loop A
                            continue;
                if (satisfy)
                {
                    // printf("checkover, good.\n");
                    Byte *recData = new Byte[recLen];
                    // resultRecDatas->push_back(new Byte[recLen]);
                    if (columnList != nullptr)
                    {
                        int offset = (columnList->size() + 7) / 8;
                        for (const Expr &columnExpr : *columnList) // expr like A.a
                        {
                            if (NameUtil::cmp(columnExpr._tbName.c_str(), tableNameB) == 0)
                            {
                                int columnID = _sm->_getColumnID(tbBID, columnExpr._colName.c_str());
                                const Column &columnB = tbBMeta._columns[columnID];
                                memcpy(recData + offset,
                                       recDataB + offsetBs[columnID], columnB._attrLen);
                                offset += columnB._attrLen;
                            }
                            else
                            {
                                int columnID = _sm->_getColumnID(tbAID, columnExpr._colName.c_str());
                                const Column &columnA = tbAMeta._columns[columnID];
                                memcpy(recData + offset,
                                       recDataA + offsetAs[columnID], columnA._attrLen);
                                offset += columnA._attrLen;
                            }
                        }
                    }
                    else // select *
                    {
                        int offset = (tbAMeta._columnNum + tbBMeta._columnNum + 7) / 8;
                        for (int i = 0; i <= tbAMeta._columnNum - 1; i++)
                        {
                            const Column &columnA = tbAMeta._columns[i];
                            memcpy(recData + offset,
                                   recDataA + offsetAs[i], columnA._attrLen);
                            offset += columnA._attrLen;
                        }
                        for (int i = 0; i <= tbBMeta._columnNum - 1; i++)
                        {
                            const Column &columnB = tbBMeta._columns[i];
                            memcpy(recData + offset,
                                   recDataB + offsetBs[i], columnB._attrLen);
                            offset += columnB._attrLen;
                        }
                    }
                    resultRecDatas->push_back(recData);
                }
                // else
                //     printf("checkover, bad.\n");
            }
        }
        recScanner.closeScan();
    }
    _sm->_rm->closeRecFile(recHandler);
    if (key != nullptr)
        delete[] key;
    delete[] recDataB;
    delete[] offsetAs;
    delete[] offsetBs;
    return resultRecDatas;
}

int *QueryManager::_getOffsets(const TableMeta &tbMeta) const
{
    int *offsets = new int[tbMeta._columnNum]{tbMeta.nullByteLen()};
    for (int i = 1; i <= tbMeta._columnNum - 1; i++)
        offsets[i] = offsets[i - 1] + tbMeta._columns[i - 1]._attrLen;
    return offsets;
}

void QueryManager::_showRec(const TableMeta &tbMeta, const int *offsets, const Byte *recData,
                            const std::vector<int> *columnIDs) const
{
    if (columnIDs == nullptr) // select *
    {
        for (int i = 0; i <= tbMeta._columnNum - 1; i++)
            switch (tbMeta._columns[i]._attrType)
            {
            case INT:
                printf("%-16d |", *((int *)(recData + offsets[i])));
                break;
            case FLOAT:
                printf("%-16.2f |", *((float *)(recData + offsets[i])));
                break;
            case VARCHAR:
            {
                int len = tbMeta._columns[i]._attrLen > 16 ? 16 : tbMeta._columns[i]._attrLen;
                printf("%-16.*s |", len, recData + offsets[i]);
                break;
            }
            default:
                break;
            }
        printf("\n");
    }
    else
    {
        for (int columnID : *columnIDs)
            switch (tbMeta._columns[columnID]._attrType)
            {
            case INT:
                printf("%-16d |", *((int *)(recData + offsets[columnID])));
                break;
            case FLOAT:
                printf("%-16.2f |", *((float *)(recData + offsets[columnID])));
                break;
            case VARCHAR:
            {
                int len = tbMeta._columns[columnID]._attrLen > 16 ? 16 : tbMeta._columns[columnID]._attrLen;
                printf("%-16.*s |", len, recData + offsets[columnID]);
                break;
            }
            default:
                break;
            }
        printf("\n");
    }
}

bool QueryManager::insertRec(const char *tableName, const std::vector<Expr> *valList, bool safe) const
{
    // printf("INTO ::INSERT\n");
    int tbID = _sm->_getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        return false;
    }
    Byte *recData = new Byte[_sm->_getRecLen(tbID)]{};
    if (!_sm->_setRecData(recData, tbID, valList))
    {
        delete[] recData;
        return false;
    }
    // printf("Set data to insert well.\n");
    if (!safe)
        if (!_sm->_checkConstraintWhenInsert(recData, tbID))
        {
            delete[] recData;
            return false;
        }
    // printf("Can insert.\n");
    // insert
    const TableMeta &tbMeta = _sm->_DBMeta._tbMetas[tbID];
    RecordHandler recHandler;
    _sm->_rm->openRecFile(NameUtil::recFileName(_sm->_DBName.c_str(), tbMeta._name).c_str(), recHandler);
    RID rid(-1, -1);
    recHandler.insertRec(recData, rid);
    // printf("Insert (pid %d, sid %d) in REC.\n", rid._pageID, rid._slotID);
    _sm->_rm->closeRecFile(recHandler);
    // insert in index file
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (tbMeta._indexes[i]._exist)
        {
            int keyOffset = tbMeta.nullByteLen();
            int keyLen;
            for (int j = 0; j <= tbMeta._columnNum - 1; j++)
            {
                if (j == tbMeta._indexes[i]._columnID)
                {
                    keyLen = tbMeta._columns[j]._attrLen;
                    break;
                }
                keyOffset += tbMeta._columns[j]._attrLen;
            }
            IndexHandler idxHandler;
            // printf("------BEFORE OPEN IDX--------\n");
            _sm->_im->openIndexFile(tbMeta._name, i, idxHandler);
            // printf("------AFTER--------------\n");
            idxHandler.insertEntry(rid, recData + keyOffset);
            // printf("Insert KEY %d (pid %d, sid %d) in IDX.\n", *((int *)(recData + keyOffset)), rid._pageID, rid._slotID);
            _sm->_im->closeIndexFile(idxHandler);
        }
    delete[] recData;
    // printf("FINISH.\n_________________________________________\n");
    return true;
}

bool QueryManager::safeInsert(const char *tableName, const std::vector<std::vector<Expr>> *valLists) const
{
    clock_t start = clock();
    int success = 0;
    int tbID = _sm->_getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    // insert
    const TableMeta &tbMeta = _sm->_DBMeta._tbMetas[tbID];
    RecordHandler recHandler;
    _sm->_rm->openRecFile(NameUtil::recFileName(_sm->_DBName.c_str(), tbMeta._name).c_str(), recHandler);
    RID rid(-1, -1);
    Byte *recData = new Byte[_sm->_getRecLen(tbID)]{};
    std::vector<IndexHandler *> idxHandlers;
    std::vector<int> offsets;
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (tbMeta._indexes[i]._exist)
        {
            IndexHandler *idxHandler = new IndexHandler;
            _sm->_im->openIndexFile(tbMeta._name, i, *idxHandler);
            idxHandlers.push_back(idxHandler);
            int keyOffset = tbMeta.nullByteLen();
            for (int j = 0; j <= tbMeta._columnNum - 1; j++)
            {
                if (j == tbMeta._indexes[i]._columnID)
                    break;
                keyOffset += tbMeta._columns[j]._attrLen;
            }
            offsets.push_back(keyOffset);
        }
    for (const std::vector<Expr> &valList : *valLists)
    {
        _sm->_setRecData(recData, tbID, &valList, true);
        if (!recHandler.insertRec(recData, rid))
            continue;
        else
            success++;
        // insert in index file
        for (int i = 0; i <= idxHandlers.size() - 1; i++)
        {
            if (!idxHandlers[i]->insertEntry(rid, recData + offsets[i]))
                continue;
        }
    }
    _sm->_rm->closeRecFile(recHandler);
    for (IndexHandler *idxHandler : idxHandlers)
    {
        _sm->_im->closeIndexFile(*idxHandler);
        delete idxHandler;
    }
    delete[] recData;
    printf("Successfully insert %d record of %d.\n", success, (int)(valLists->size()));
    printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    return true;
}

bool QueryManager::deleteRec(const char *tableName, const std::vector<Expr> *condList) const
{
    clock_t start = clock();
    int tbID = _sm->_getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        printf("Successfully delete %d record.\n", 0);
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    if (!_checkCondList(tableName, tbID, condList))
    {
        printf("Successfully delete %d record.\n", 0);
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    int dataLen = _sm->_getRecLen(tbID);
    // Byte *recData = new Byte[_sm->_getRecLen(tbID)];
    std::vector<RID> rids = _filter(tableName, tbID, condList);
    const TableMeta &tbMeta = _sm->_DBMeta._tbMetas[tbID];
    RecordHandler recHandler;
    _sm->_rm->openRecFile(NameUtil::recFileName(_sm->_DBName.c_str(), tableName).c_str(), recHandler);
    std::vector<Byte *> recDatas;
    std::vector<RID> deletedRids;
    for (const RID &rid : rids)
    {
        Byte *recData = new Byte[dataLen];
        if (!recHandler.accessRec(rid, recData))
        {
            delete[] recData;
            continue;
        }
        if (!_sm->_checkConstraintWhenDelete(tableName, tbID, recData))
        {
            delete[] recData;
            continue;
        }
        if (!recHandler.deleteRec(rid))
        {
            delete[] recData;
            continue;
        }
        recDatas.push_back(recData);
        deletedRids.push_back(rid);
    }
    _sm->_rm->closeRecFile(recHandler);
    if (deletedRids.empty())
    {
        printf("Successfully delete %d record.\n", 0);
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    // delete in index file
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (tbMeta._indexes[i]._exist)
        {
            int keyOffset = tbMeta.nullByteLen();
            int keyLen = tbMeta._columns[tbMeta._indexes[i]._columnID]._attrLen;
            for (int j = 0; j <= tbMeta._columnNum - 1; j++)
            {
                if (j == tbMeta._indexes[i]._columnID)
                    break;
                keyOffset += tbMeta._columns[j]._attrLen;
            }
            IndexHandler idxHandler;
            _sm->_im->openIndexFile(tbMeta._name, i, idxHandler);
            for (int i = 0; i <= deletedRids.size() - 1; i++)
            {
                // printf("Delete (pid %d, sid %d) in idxfile.\n", rids[i]._pageID, rids[i]._slotID);
                idxHandler.deleteEntry(deletedRids[i], recDatas[i] + keyOffset);
            }
            _sm->_im->closeIndexFile(idxHandler);
        }
    for (const Byte *recData : recDatas)
        delete[] recData;
    printf("Successfully delete %d record.\n", (int)(deletedRids.size()));
    printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    return true;
}

bool QueryManager::updateRec(const char *tableName,
                             const std::vector<Expr> *setList,
                             const std::vector<Expr> *condList) const
{
    clock_t start = clock();
    int tbID = _sm->_getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        printf("Successfully update %d record.\n", 0);
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    std::vector<int> columnIDs;
    if (!_checkSetList(tableName, tbID, setList, &columnIDs))
    {
        printf("Successfully update %d record.\n", 0);
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    if (!_checkCondList(tableName, tbID, condList))
    {
        printf("Successfully update %d record.\n", 0);
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    std::vector<RID> rids = _filter(tableName, tbID, condList);
    RecordHandler recHandler;
    _sm->_rm->openRecFile(NameUtil::recFileName(_sm->_DBName.c_str(), tableName).c_str(), recHandler);
    const TableMeta &tbMeta = _sm->_DBMeta._tbMetas[tbID];
    int *offsets = _getOffsets(tbMeta);
    std::vector<Byte *> newRecDatas;
    std::vector<Byte *> oldRecDatas;
    std::vector<RID> updatedRids;
    for (const RID &rid : rids)
    {
        Byte *newRecData = new Byte[recHandler._fHeader._slotLen];
        Byte *oldRecData = new Byte[recHandler._fHeader._slotLen];
        if (!recHandler.accessRec(rid, oldRecData))
        {
            delete[] newRecData;
            delete[] oldRecData;
            continue;
        }
        // set new recData
        memcpy(newRecData, oldRecData, recHandler._fHeader._slotLen);
        for (int i = 0; i <= setList->size() - 1; i++) // expr like A.a = val
            memcpy(newRecData + offsets[columnIDs[i]], (*setList)[i]._r->_val._val(),
                   tbMeta._columns[columnIDs[i]]._attrLen);
        if (!_sm->_checkConstraintWhenUpdate(tableName, tbID, oldRecData, newRecData, &columnIDs))
        {
            delete[] newRecData;
            delete[] oldRecData;
            continue;
        }
        if (!recHandler.updateRec(rid, newRecData))
        {
            delete[] newRecData;
            delete[] oldRecData;
            continue;
        }
        newRecDatas.push_back(newRecData);
        oldRecDatas.push_back(oldRecData);
        updatedRids.push_back(rid);
    }
    _sm->_rm->closeRecFile(recHandler);
    if (updatedRids.empty())
    {
        printf("Successfully update %d record.\n", 0);
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    // delete in index file
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (tbMeta._indexes[i]._exist)
        {
            bool change = false;
            for (int columnID : columnIDs)
                if (columnID == tbMeta._indexes[i]._columnID)
                    change = true;
            if (!change)
                continue;
            int keyOffset = tbMeta.nullByteLen();
            int keyLen = tbMeta._columns[tbMeta._indexes[i]._columnID]._attrLen;
            for (int j = 0; j <= tbMeta._columnNum - 1; j++)
            {
                if (j == tbMeta._indexes[i]._columnID)
                    break;
                keyOffset += tbMeta._columns[j]._attrLen;
            }
            IndexHandler idxHandler;
            _sm->_im->openIndexFile(tbMeta._name, i, idxHandler);
            for (int i = 0; i <= updatedRids.size() - 1; i++)
            {
                // printf("Delete (pid %d, sid %d) in idxfile.\n", rids[i]._pageID, rids[i]._slotID);
                idxHandler.deleteEntry(updatedRids[i], oldRecDatas[i] + keyOffset);
                idxHandler.insertEntry(updatedRids[i], newRecDatas[i] + keyOffset);
            }
            _sm->_im->closeIndexFile(idxHandler);
        }
    for (const Byte *newRecData : newRecDatas)
        delete[] newRecData;
    for (const Byte *oldRecData : oldRecDatas)
        delete[] oldRecData;
    delete[] offsets;
    printf("Successfully update %d record.\n", (int)(updatedRids.size()));
    printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    return true;
}

bool QueryManager::selectRec(bool join, const char *tableNameA, const char *tableNameB,
                             const std::vector<Expr> *columnList,
                             std::vector<Expr> *condList) const
{
    clock_t start = clock();
    if (join)
    {
        int tbAID = _sm->_getTableID(tableNameA);
        if (tbAID < 0) // table not existing
        {
            printf("Non-existing table A.\n");
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return false;
        }
        int tbBID = _sm->_getTableID(tableNameB);
        if (tbBID < 0) // table not existing
        {
            printf("Non-existing table B.\n");
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return false;
        }
        if (!_checkColumnListWhenJoin(tableNameA, tableNameB, tbAID, tbBID, columnList))
        {
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return false;
        }
        if (!_checkCondListWhenJoin(tableNameA, tableNameB, tbAID, tbBID, condList))
        {
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return false;
        }
        std::vector<RID> ridAs = _filter(tableNameA, tbAID, condList, true); // filter called by join
        std::vector<Byte *> recDataAs;
        RecordHandler recHandler;
        _sm->_rm->openRecFile(NameUtil::recFileName(_sm->_DBName.c_str(), tableNameA).c_str(), recHandler);
        for (RID &rid : ridAs)
        {
            Byte *recData = new Byte[recHandler._fHeader._slotLen];
            if (!recHandler.accessRec(rid, recData))
                continue;
            recDataAs.push_back(recData);
        }
        _sm->_rm->closeRecFile(recHandler);
        // prepare table for result
        TableMeta resultTbMeta;
        NameUtil::cpy(resultTbMeta._name, "join result table");
        resultTbMeta._columnNum = 0;
        std::vector<Byte *> *resultRecDatas = _join(tableNameA, tableNameB, tbAID, tbBID,
                                                    columnList, condList, &recDataAs, resultTbMeta);
        printf("       | ");
        for (int i = 0; i <= resultTbMeta._columnNum - 1; i++)
            printf("%-16.16s |", resultTbMeta._columns[i]._name);
        printf("\n");
        if (!resultRecDatas->empty())
        {
            int *offsets = _getOffsets(resultTbMeta);
            int col = 1;
            for (const Byte *recData : *resultRecDatas)
            {
                printf("%-7d| ", col);
                _showRec(resultTbMeta, offsets, recData, nullptr); // set nullptr to show all columns
                col++;
            }
            delete[] offsets;
        }
        for (Byte *&recData : recDataAs)
            delete[] recData;
        for (Byte *&recData : *resultRecDatas)
            delete[] recData;
        delete resultRecDatas;
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return true;
    }
    else
    {
        int tbID = _sm->_getTableID(tableNameA);
        if (tbID < 0) // table not existing
        {
            printf("Non-existing table.\n");
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return false;
        }
        std::vector<int> columnIDs;
        if (!_checkColumnList(tableNameA, tbID, columnList, &columnIDs))
        {
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return false;
        }
        if (!_checkCondList(tableNameA, tbID, condList))
        {
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return false;
        }
        std::vector<RID> rids = _filter(tableNameA, tbID, condList);
        RecordHandler recHandler;
        _sm->_rm->openRecFile(NameUtil::recFileName(_sm->_DBName.c_str(), tableNameA).c_str(), recHandler);
        Byte *recData = new Byte[recHandler._fHeader._slotLen];
        const TableMeta &tbMeta = _sm->_DBMeta._tbMetas[tbID];
        int *offsets = _getOffsets(tbMeta);
        printf("       | ");
        if (columnIDs.empty())
            for (int i = 0; i <= tbMeta._columnNum - 1; i++)
                printf("%-16.16s |", tbMeta._columns[i]._name);
        else
            for (int columnID : columnIDs)
                printf("%-16.16s |", tbMeta._columns[columnID]._name);
        printf("\n");
        int col = 1;
        for (const RID &rid : rids)
        {
            if (!recHandler.accessRec(rid, recData))
                continue;
            printf("%-7d| ", col);
            if (columnIDs.empty()) // select *, due to columnList == nullptr
                _showRec(tbMeta, offsets, recData, nullptr);
            else
                _showRec(tbMeta, offsets, recData, &columnIDs);
            col++;
        }
        _sm->_rm->closeRecFile(recHandler);
        delete[] offsets;
        delete[] recData;
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return true;
    }
}
