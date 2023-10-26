#include "SystemManager.h"

// expr like A.a op val, A.a op A.b
bool SystemManager::_satisfy(const Expr &condExpr, int tbID, const int *offsets, const Byte *recData) const
{
    const TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    int lColumnID = _getColumnID(tbID, condExpr._l->_colName.c_str());
    if (condExpr._r->_isVal) // A.a op val
        return CmpUtil::match(condExpr._r->_val._type,
                              recData + offsets[lColumnID], condExpr._op, condExpr._r->_val._val(),
                              tbMeta._columns[lColumnID]._attrLen);
    int rColumnID = _getColumnID(tbID, condExpr._r->_colName.c_str());
    return CmpUtil::match(tbMeta._columns[rColumnID]._attrType,
                          recData + offsets[lColumnID], condExpr._op, recData + offsets[rColumnID],
                          tbMeta._columns[lColumnID]._attrLen);
}

// expr like A.a op B.b
bool SystemManager::_satisfy(const Expr &condExpr, int lTbID, int rTbID, const int *lOffsets, const int *rOffsets,
                             const Byte *lRecData, const Byte *rRecData) const
{
    const TableMeta &lTbMeta = _DBMeta._tbMetas[lTbID];
    const TableMeta &rTbMeta = _DBMeta._tbMetas[rTbID];
    int lColumnID = _getColumnID(lTbID, condExpr._l->_colName.c_str());
    int rColumnID = _getColumnID(rTbID, condExpr._r->_colName.c_str());
    return CmpUtil::match(lTbMeta._columns[lColumnID]._attrType,
                          lRecData + lOffsets[lColumnID], condExpr._op,
                          rRecData + rOffsets[rColumnID],
                          lTbMeta._columns[lColumnID]._attrLen);
}

int SystemManager::_getTableID(const char *tableName) const
{
    for (int i = 0; i <= MAX_TABLE_NUM - 1; i++)
        if (NameUtil::cmp(tableName, _DBMeta._tbNames[i]) == 0)
            return i;
    return -1;
}

// must make sure this table exists
int SystemManager::_getColumnID(int tbID, const char *columnName) const
{
    for (int i = 0; i <= _DBMeta._tbMetas[tbID]._columnNum - 1; i++)
        if (NameUtil::cmp(columnName, _DBMeta._tbMetas[tbID]._columns[i]._name) == 0)
            return i;
    return -1;
}

bool SystemManager::_loadDBMeta(const char *DBName)
{
    int DBMetaFileID;
    std::string DBMetaFileName = NameUtil::DBMetaFileName(DBName);
    if (!_fm->openFile(DBMetaFileName.c_str(), DBMetaFileID))
        return false;
    // load meta info from file
    int index;
    BufType pageBuf = _bpm->getPage(DBMetaFileID, 0, index);
    memcpy((void *)(&_DBMeta), pageBuf, sizeof(DBMeta));
    _bpm->access(index);
    _DBName = std::string(DBName);
    return true;
}

void SystemManager::_writeBackSysMeta() const
{
    int index;
    BufType pageBuf = _bpm->getPage(_sysFileID, 0, index);
    memcpy(pageBuf, &_sysMeta, sizeof(SystemMeta));
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _fm->closeFile(_sysFileID);
}

void SystemManager::_writeBackDBMeta() const
{
    // printf("Write back db %s.\n", _DBName.c_str());
    int fileID;
    if (!_fm->openFile(NameUtil::DBMetaFileName(_DBName.c_str()).c_str(), fileID))
        return;
    int index;
    BufType pageBuf = _bpm->getPage(fileID, 0, index);
    memcpy(pageBuf, &_DBMeta, sizeof(DBMeta));
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _fm->closeFile(fileID);
}

// for QM
bool SystemManager::_foundInPrimaryKey(const Byte *cmpKey, int priTbID) const
{
    const TableMeta &tbMeta = _DBMeta._tbMetas[priTbID];
    IndexHandler idxHandler;
    _im->openIndexFile(tbMeta._name, tbMeta._primaryKey._indexID, idxHandler);
    IndexScanner idxScanner;
    // printf("idx.openScan to find PRIMARY KEY = %d.\n", *((int *)cmpKey));
    idxScanner.openScan(&idxHandler, cmpKey, EQ);
    RID rid(-1, -1);
    bool result = idxScanner.getNextEntry(rid);
    idxScanner.closeScan();
    _im->closeIndexFile(idxHandler);
    return result;
}

bool SystemManager::_foundInForeignKey(const Byte *cmpKey, const char *forTbName, int forTbID, int forColumnID) const
{
    const TableMeta &tbMeta = _DBMeta._tbMetas[forTbID];
    int keyOffset = tbMeta.nullByteLen();
    for (int i = 0; i <= tbMeta._columnNum - 1; i++)
    {
        if (i == forColumnID)
            break;
        keyOffset += tbMeta._columns[i]._attrLen;
    }
    // fry finding a index
    int indexID = -1;
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (tbMeta._indexes[i]._exist && forColumnID == tbMeta._indexes[i]._columnID)
        {
            indexID = i;
            break;
        }
    if (indexID != -1)
    {
        IndexHandler idxHandler;
        _im->openIndexFile(tbMeta._name, indexID, idxHandler);
        IndexScanner idxScanner;
        idxScanner.openScan(&idxHandler, cmpKey, EQ);
        // printf("idx.openScan to find FOREIGN KEY = %d.\n", *((int *)cmpKey));
        RID rid(-1, -1);
        bool result = idxScanner.getNextEntry(rid);
        idxScanner.closeScan();
        _im->closeIndexFile(idxHandler);
        return result;
    }
    // no index
    RecordHandler recHandler;
    _rm->openRecFile(NameUtil::recFileName(_DBName.c_str(), forTbName).c_str(), recHandler);
    RID rid(-1, -1);
    Byte *recData = new Byte[recHandler._fHeader._slotLen];
    RecordScanner recScanner;
    recScanner.openScan(&recHandler);
    // printf("rec.openScan to find FOREIGN KEY = %d.\n", *((int *)cmpKey));
    bool result = false;
    while (recScanner.getNextRecord(rid, recData))
        if (CmpUtil::match(tbMeta._columns[forColumnID]._attrType,
                           cmpKey, EQ, recData + keyOffset,
                           tbMeta._columns[forColumnID]._attrLen))
        {
            result = true;
            break;
        }
    recScanner.closeScan();
    _rm->closeRecFile(recHandler);
    delete[] recData;
    return result;
}

// must make sure this table exists
bool SystemManager::_checkConstraintWhenInsert(const Byte *recData, int tbID) const
{
    const TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    // check for primary key
    if (tbMeta._primaryKey._exist)
    {
        // duplicate attr on primary key is unacceptable
        int keyOffset = tbMeta.nullByteLen();
        for (int i = 0; i <= tbMeta._columnNum - 1; i++)
        {
            if (i == tbMeta._primaryKey._columnID)
                break;
            keyOffset += tbMeta._columns[i]._attrLen;
        }
        // printf("We will search duplicate attr %d on pk.\n", *((int *)(recData + keyOffset)));
        if (_foundInPrimaryKey(recData + keyOffset, tbID))
        {
            printf("Duplicate attr on primary key is unacceptable.\n");
            return false;
        }
    }
    // check for foreign key
    for (int i = 0; i <= MAX_FOREIGN_KEY_NUM - 1; i++)
        if (tbMeta._foreignKeys[i]._priTbID != -1)
        {
            // printf("pri tb %s\n", _DBMeta._tbNames[tbMeta._foreignKeys[i]._priTbID]);
            // printf("pri col %s\n", _DBMeta._tbMetas[tbMeta._foreignKeys[i]._priTbID]._columns[tbMeta._foreignKeys[i]._priColumnID]._name);
            // printf("for col %s\n", tbMeta._columns[tbMeta._foreignKeys[i]._forColumnID]._name);
            // check for null
            // if (NullUtil::isNull(recData, tbMeta._foreignKeys[i]._forColumnID))
            // {
            //     continue; // no need to check other table's primary key
            // }
            int priTbID = tbMeta._foreignKeys[i]._priTbID;
            int keyOffset = tbMeta.nullByteLen();
            for (int j = 0; j <= tbMeta._columnNum - 1; j++)
            {
                if (j == tbMeta._foreignKeys[i]._forColumnID)
                    break;
                keyOffset += tbMeta._columns[j]._attrLen;
            }
            // printf("SEARCH REFERENCE.\n");
            if (!_foundInPrimaryKey(recData + keyOffset, priTbID))
            // no same attr on primary key is unacceptable
            {
                printf("No same attr on referred table's primary key is unacceptable.\n");
                return false;
            }
        }
    return true;
}

// must make sure this table exists
bool SystemManager::_checkConstraintWhenDelete(const char *tableName, int tbID, const Byte *recData, bool callByUpdate) const
{
    const TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    // check for primary key
    if (tbMeta._primaryKey._exist)
    {
        int keyOffset = tbMeta.nullByteLen();
        for (int i = 0; i <= tbMeta._columnNum - 1; i++)
        {
            if (i == tbMeta._primaryKey._columnID)
                break;
            keyOffset += tbMeta._columns[i]._attrLen;
        }
        // deleting when primary key has references is unacceptable
        for (int i = 0; i <= MAX_REFER_NUM - 1; i++)
            if (tbMeta._primaryKey._refers[i]._priTbID != -1)
            {
                // printf("pri tb %s\n", _DBMeta._tbNames[tbMeta._primaryKey._refers[i]._priTbID]);
                // printf("pri col %s\n", _DBMeta._tbMetas[tbMeta._primaryKey._refers[i]._priTbID]._columns[tbMeta._primaryKey._refers[i]._priColumnID]._name);
                // printf("for col %s\n", tbMeta._columns[tbMeta._primaryKey._refers[i]._forColumnID]._name);
                int forTbID = tbMeta._primaryKey._refers[i]._forTbID;
                // printf("for tb %s\n", _DBMeta._tbNames[forTbID]);
                if (_foundInForeignKey(recData + keyOffset,
                                       _DBMeta._tbNames[tbMeta._primaryKey._refers[i]._forTbID],
                                       tbMeta._primaryKey._refers[i]._forTbID,
                                       tbMeta._primaryKey._refers[i]._forColumnID))
                // deleting when primary key has references is unacceptable
                {
                    if (!callByUpdate)
                        printf("Deleting when primary key has references is unacceptable.\n");
                    return false;
                }
            }
    }
    return true;
}

bool SystemManager::_checkConstraintWhenUpdate(const char *tableName, int tbID,
                                               const Byte *oldRecData, const Byte *newRecData,
                                               std::vector<int> *columnIDs) const
{
    const TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    for (int columnID : *columnIDs)
    {
        if (tbMeta._primaryKey._exist && columnID == tbMeta._primaryKey._columnID) // update primary key
        {
            int keyOffset = tbMeta.nullByteLen();
            for (int i = 0; i <= tbMeta._columnNum - 1; i++)
            {
                if (i == tbMeta._primaryKey._columnID)
                    break;
                keyOffset += tbMeta._columns[i]._attrLen;
            }
            // deleting when primary key has references is unacceptable
            for (int i = 0; i <= MAX_REFER_NUM - 1; i++)
                if (tbMeta._primaryKey._refers[i]._priTbID != -1)
                {
                    // printf("pri tb %s\n", _DBMeta._tbNames[tbMeta._primaryKey._refers[i]._priTbID]);
                    // printf("pri col %s\n", _DBMeta._tbMetas[tbMeta._primaryKey._refers[i]._priTbID]._columns[tbMeta._primaryKey._refers[i]._priColumnID]._name);
                    // printf("for col %s\n", tbMeta._columns[tbMeta._primaryKey._refers[i]._forColumnID]._name);
                    int forTbID = tbMeta._primaryKey._refers[i]._forTbID;
                    // printf("for tb %s\n", _DBMeta._tbNames[forTbID]);
                    if (_foundInForeignKey(oldRecData + keyOffset,
                                           _DBMeta._tbNames[tbMeta._primaryKey._refers[i]._forTbID],
                                           tbMeta._primaryKey._refers[i]._forTbID,
                                           tbMeta._primaryKey._refers[i]._forColumnID))
                    // deleting when primary key has references is unacceptable
                    {
                        printf("Updating primary key when it has references is unacceptable.\n");
                        return false;
                    }
                }
            // duplicate attr on primary key is unacceptable
            if (_foundInPrimaryKey(newRecData + keyOffset, tbID))
            {
                printf("Duplicate attr on primary key is unacceptable.\n");
                return false;
            }
        }
        for (int i = 0; i <= MAX_FOREIGN_KEY_NUM - 1; i++)
        {
            if (tbMeta._foreignKeys[i]._priTbID != -1 && columnID == tbMeta._foreignKeys->_forColumnID)
            {
                // if (NullUtil::isNull(newRecData, tbMeta._foreignKeys[i]._forColumnID))
                // {
                //     continue;// no need to check other table's primary key
                // }
                int priTbID = tbMeta._foreignKeys[i]._priTbID;
                int keyOffset = tbMeta.nullByteLen();
                for (int j = 0; j <= tbMeta._columnNum - 1; j++)
                {
                    if (j == tbMeta._foreignKeys[i]._forColumnID)
                        break;
                    keyOffset += tbMeta._columns[j]._attrLen;
                }
                // printf("SEARCH REFERENCE.\n");
                if (!_foundInPrimaryKey(newRecData + keyOffset, priTbID))
                // no same attr on primary key is unacceptable
                {
                    printf("No same attr on referred table's primary key is unacceptable.\n");
                    return false;
                }
            }
        }
    }
    return true;
}

// must make sure this table exists
int SystemManager::_getRecLen(int tbID) const
{
    const TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    int recLen = tbMeta.nullByteLen();
    for (int i = 0; i <= tbMeta._columnNum - 1; i++)
        recLen += tbMeta._columns[i]._attrLen;
    return recLen;
}

// must make sure this table exists
bool SystemManager::_setRecData(Byte *recData, int tbID, const std::vector<Expr> *valList, bool safe) const
{
    const TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    if (!safe)
    {
        if (tbMeta._columnNum != valList->size())
        {
            printf("col: %d, you give %d\n", tbMeta._columnNum, (int)(valList->size()));
            printf("Number of columns and values mismatch.\n");
            return false;
        }
        for (int i = 0; i <= tbMeta._columnNum - 1; i++)
            if ((*valList)[i]._val._val() != nullptr && (*valList)[i]._val._type != tbMeta._columns[i]._attrType)
            {
                printf("Attrtype mismatch.\n");
                return false;
            }
    }
    int attrOffset = tbMeta.nullByteLen();
    for (int i = 0; i <= tbMeta._columnNum - 1; i++)
    {
        const Byte *val = (*valList)[i]._val._val();
        if (val != nullptr) // not null
        {
            NullUtil::setNullBit(recData, i, false);
            memcpy(recData + attrOffset, (*valList)[i]._val._val(), tbMeta._columns[i]._attrLen);
        }
        else // set null bit to 1
        {
            if (tbMeta._columns[i]._canBeNull)
                NullUtil::setNullBit(recData, i, true);
            else
            {
                printf("Can't insert NULL into not null column.\n");
                return false;
            }
        }
        attrOffset += tbMeta._columns[i]._attrLen;
    }
    return true;
}

// system
void SystemManager::showDBs() const
{
    bool first = true;
    for (int i = 0; i <= MAX_DB_NUM - 1; i++)
        if (strlen(_sysMeta._DBNames[i]) != 0)
        {
            if (!first)
                printf(", ");
            else
                first = false;
            printf("%-16.16s", _sysMeta._DBNames[i]);
        }
}

// DB
bool SystemManager::createDB(const char *DBName)
{
    for (int i = 0; i <= MAX_DB_NUM - 1; i++)
        if (NameUtil::cmp(DBName, _sysMeta._DBNames[i]) == 0)
        {
            printf("DB with the same name already exists.\n");
            return false;
        }
    for (int i = 0; i <= MAX_DB_NUM - 1; i++)
        if (strlen(_sysMeta._DBNames[i]) == 0)
        {
            NameUtil::cpy(_sysMeta._DBNames[i], DBName);
            if (!_fm->createFile(NameUtil::DBMetaFileName(DBName).c_str()))
                return false;
            _DBName = std::string(DBName);
            _writeBackDBMeta();
            // _writeBackSysMeta();
            printf("Create DB well.\n");
            return true;
        }
    printf("Can't create more DB.\n");
    return false;
}

bool SystemManager::dropDB(const char *DBName)
{
    if (!_usingDB)
    {
        printf("Haven't use certain db.\n");
        return false;
    }
    for (int i = 0; i <= MAX_DB_NUM - 1; i++)
        if (NameUtil::cmp(DBName, _sysMeta._DBNames[i]) == 0)
        {
            for (int j = 0; j <= MAX_TABLE_NUM - 1; j++)
                if (strlen(_DBMeta._tbNames[j]) != 0)
                {
                    if (!dropTable(_DBMeta._tbNames[j], true))
                        return false;
                }
            _bpm->close();
            std::string rmCmd = "rm " + NameUtil::DBMetaFileName(DBName);
            system(rmCmd.c_str());
            memset(_sysMeta._DBNames[i], 0, MAX_NAME_LEN);
            printf("Drop DB well.\n");
            return true;
        }
    printf("No matched DB.\n");
    return false;
}

bool SystemManager::useDB(const char *DBName)
{
    if (_usingDB)
        _writeBackDBMeta();
    if (_loadDBMeta(DBName))
    {
        _usingDB = true;
        printf("Use DB well.\n");
        return true;
    }
    printf("Non-existing db.\n");
    return false;
}

void SystemManager::showUse() const
{
    if (_usingDB)
        printf("%-16.16s", _DBName.c_str());
    else
        printf("No using DB.\n");
}

bool SystemManager::showTables() const
{
    if (!_usingDB)
    {
        printf("Haven't use certain db.\n");
        return false;
    }
    for (int i = 0; i <= MAX_TABLE_NUM - 1; i++)
        if (strlen(_DBMeta._tbNames[i]) != 0)
            printf("%-16.16s\n", _DBMeta._tbNames[i]);
    return true;
}

// Table
bool SystemManager::createTable(const char *tableName, const std::vector<Column> *columnList)
{
    if (!_usingDB)
    {
        printf("Haven't use certain db.\n");
        return false;
    }
    for (int i = 0; i <= MAX_TABLE_NUM - 1; i++)
        if (NameUtil::cmp(tableName, _DBMeta._tbNames[i]) == 0)
        {
            printf("Table with the same name already exists.\n");
            return false;
        }
    if (columnList->size() > MAX_COL_NUM)
    {
        printf("Too many columns.\n");
        return false;
    }
    for (int i = 0; i <= MAX_TABLE_NUM - 1; i++)
        if (strlen(_DBMeta._tbNames[i]) == 0)
        {
            TableMeta tbMeta;
            NameUtil::cpy(tbMeta._name, tableName);
            tbMeta._columnNum = columnList->size();
            // printf("L = %d\n", columnList->size());
            int recLen = tbMeta.nullByteLen();
            for (int j = 0; j <= columnList->size() - 1; j++)
            {
                tbMeta._columns[j] = (*columnList)[j];
                recLen += (*columnList)[j]._attrLen;
            }
            _DBMeta._tbMetas[i] = tbMeta;
            NameUtil::cpy(_DBMeta._tbNames[i], tableName);
            _rm->createRecFile(NameUtil::recFileName(_DBName.c_str(), tableName).c_str(), recLen, tbMeta._columnNum);
            // // add primary key
            // for (const Column &column : *columnList)
            //     if (column._isPriKey)
            //     {
            //         if (!addPrimaryKey(tableName, column._name, true))
            //             return false;
            //     }
            // // add foreign key
            // for (const Column &column : *columnList)
            //     if (column._hasForKey)
            //     {
            //         if (!addForeignKey(_DBMeta._tbNames[column._referPriTbID], tableName, column._name,
            //                            _DBMeta._tbNames[column._referPriTbID],
            //                            _DBMeta._tbMetas[column._referPriTbID]._columns[column._referColumnID]._name, true))
            //             return false;
            //     }
            _writeBackDBMeta();
            printf("Create table well.\n");
            return true;
        }
    printf("Table number limit.\n");
    return false;
}

bool SystemManager::dropTable(const char *tableName, bool callByDropTable)
{
    if (!_usingDB)
    {
        printf("Haven't use certain db.\n");
        return false;
    }
    int tbID = _getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        return false;
    }
    TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    if (!callByDropTable)
        for (int i = 0; i <= MAX_REFER_NUM - 1; i++)
            if (tbMeta._primaryKey._refers[i]._priTbID != -1)
            {
                printf("Foreign key on this table's primary key.\n");
                return false;
            }
    for (int i = 0; i <= MAX_FOREIGN_KEY_NUM - 1; i++)
        if (tbMeta._foreignKeys[i]._priTbID != -1)
        {
            TableMeta &priTbMeta = _DBMeta._tbMetas[tbMeta._foreignKeys[i]._priTbID];
            for (int j = 0; j <= MAX_REFER_NUM - 1; j++)
                if (priTbMeta._primaryKey._refers[j]._forTbID == tbID)
                    priTbMeta._primaryKey._refers[j]._priTbID = -1;
        }
    _rm->deleteRecFile(NameUtil::recFileName(_DBName.c_str(), tableName).c_str());
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (_DBMeta._tbMetas[tbID]._indexes[i]._exist)
            _im->destroyIndexFile(tbMeta._name, i);
    tbMeta = TableMeta();
    memset(_DBMeta._tbNames[tbID], 0, MAX_NAME_LEN);
    _writeBackDBMeta();
    if (!callByDropTable)
        printf("Drop table well.\n");
    return true;
}

bool SystemManager::descTable(const char *tableName) const
{
    if (!_usingDB)
    {
        printf("Haven't use certain db.\n");
        return false;
    }
    int tbID = _getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        return false;
    }
    const TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    const char splitLine[] = "----------------------------------------------------";
    printf("\n%s\n|%-16.16s|%-16.16s|%-16.16s|\n%s\n", splitLine, "Field", "Type", "Null", splitLine);
    // column info
    for (int i = 0; i <= tbMeta._columnNum - 1; i++)
    {
        // field name
        printf("|%-16.16s|", tbMeta._columns[i]._name);
        // type
        switch (tbMeta._columns[i]._attrType)
        {
        case INT:
            printf("%-16.16s|", "INT");
            break;
        case FLOAT:
            printf("%-16.16s|", "FLOAT");
            break;
        case VARCHAR:
            printf("%-16.16s|", "VARCHAR");
            break;
        default:
            break;
        }
        // null
        if (tbMeta._columns[i]._canBeNull)
            printf("%-16.16s|", "YES");
        else
            printf("%-16.16s|", "NO");
        // default
        // if (tbMeta._columns[i]._hasDefault)
        // {
        //     if (tbMeta._columns[i]._defaultVal->_val() != nullptr)
        //         switch (tbMeta._columns[i]._defaultVal->_type)
        //         {
        //         case INT:
        //             printf("%-16.16d|", *((int *)(tbMeta._columns[i]._defaultVal->_val())));
        //             break;
        //         case FLOAT:
        //             printf("%-16.2f|", *((float *)(tbMeta._columns[i]._defaultVal->_val())));
        //             break;
        //         case VARCHAR:
        //             printf("%-16.16s|", tbMeta._columns[i]._defaultVal->_val());
        //             break;
        //         default:
        //             break;
        //         }
        //     else
        //         printf("%-16.16s|", "NULL");
        // }
        // else
        //     printf("%-16.16s|", "NULL");
        printf("\n");
    }
    printf("%s\n", splitLine);
    // primary key info
    if (tbMeta._primaryKey._exist)
        printf("PRIMARY KEY (%s);\n", tbMeta._columns[tbMeta._primaryKey._columnID]._name);
    // foreign key info
    for (int i = 0; i <= MAX_FOREIGN_KEY_NUM - 1; i++)
        if (tbMeta._foreignKeys[i]._priTbID != -1)
            printf("FOREIGN KEY %s(%s) REFERENCES %s(%s);\n", tbMeta._foreignKeys[i]._name,
                   tbMeta._columns[tbMeta._foreignKeys[i]._forColumnID]._name,
                   _DBMeta._tbMetas[tbMeta._foreignKeys[i]._priTbID]._name,
                   _DBMeta._tbMetas[tbMeta._foreignKeys[i]._priTbID]._columns[tbMeta._foreignKeys[i]._priColumnID]._name);
    // index info
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (tbMeta._indexes[i]._exist)
            printf("INDEX %s(%s);\n", tbMeta._columns[tbMeta._indexes[i]._columnID]._name,
                   tbMeta._columns[tbMeta._indexes[i]._columnID]._name);
    return true;
}

// Index
bool SystemManager::addIndex(const char *tableName, const char *attrName)
{
    clock_t start = clock();
    int tbID = _getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    int columnID = _getColumnID(tbID, attrName);
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (tbMeta._indexes[i]._exist && columnID == tbMeta._indexes[i]._columnID)
        {
            printf("Index with the same name has existed.\n");
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return false;
        }
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (!tbMeta._indexes[i]._exist)
        {
            Index index;
            index._exist = true;
            index._ofPK = false;
            int columnID = _getColumnID(tbID, attrName);
            if (columnID < 0) // column not existing
            {
                printf("Non-existing column.\n");
                printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
                return false;
            }
            index._columnID = columnID;
            // printf("index %d on %dth column %s\n", i, index._columnID, attrName);
            _im->createIndexFile(tbMeta._name, i,
                                 tbMeta._columns[index._columnID]._attrType,
                                 tbMeta._columns[index._columnID]._attrLen);
            IndexHandler idxHandler;
            _im->openIndexFile(tbMeta._name, i, idxHandler);
            RecordHandler recHandler;
            _rm->openRecFile(NameUtil::recFileName(_DBName.c_str(), tableName).c_str(), recHandler);
            RecordScanner recScanner;
            int attrOffset = tbMeta.nullByteLen();
            for (int i = 0; i <= tbMeta._columnNum - 1; i++)
            {
                if (i == index._columnID)
                    break;
                attrOffset += tbMeta._columns[i]._attrLen;
            }
            recScanner.openScan(&recHandler);
            RID rid(-1, -1);
            Byte *recData = new Byte[recHandler._fHeader._slotLen];
            while (recScanner.getNextRecord(rid, recData))
                idxHandler.insertEntry(rid, recData + attrOffset);
            delete[] recData;
            recScanner.closeScan();
            _im->closeIndexFile(idxHandler);
            _rm->closeRecFile(recHandler);
            tbMeta._indexes[i] = index;
            _writeBackDBMeta();
            printf("Add index well.\n");
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return true;
        }
    printf("Index number reaches limit.\n");
    printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    return false;
}

bool SystemManager::dropIndex(const char *tableName, const char *attrName)
{
    int tbID = _getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        return false;
    }
    TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    int columnID = _getColumnID(tbID, attrName);
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (tbMeta._indexes[i]._exist && columnID == tbMeta._indexes[i]._columnID)
        {
            if (tbMeta._indexes[i]._ofPK)
            {
                printf("Can't drop index created by adding primary key.\n");
                continue;
            }
            else
            {
                _im->destroyIndexFile(tbMeta._name, i);
                memset(&tbMeta._indexes[i], 0, sizeof(Index));
                _writeBackDBMeta();
                printf("Drop index well.\n");
                return true;
            }
        }
    printf("Non-existing index.\n");
    return false;
}

// Key
bool SystemManager::addPrimaryKey(const char *tableName,
                                  const char *attrName,
                                  bool callByCreateTb)
{
    clock_t start = clock();
    int tbID = _getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    if (tbMeta._primaryKey._exist) // duplicate primary key
    {
        printf("Primary key has existed.\n");
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    PrimaryKey priKey;
    priKey._exist = true;
    int columnID = _getColumnID(tbID, attrName);
    if (columnID < 0) // column not existing
    {
        printf("Non-existing column.\n");
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        return false;
    }
    if (!callByCreateTb)
    {
        if (tbMeta._columns[columnID]._canBeNull) // can't be primary key
        {
            printf("This column can't be primary key.\n");
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return false;
        }
    }
    else
        tbMeta._columns[columnID]._canBeNull = false;
    priKey._columnID = columnID;
    // if index exist, reuse it
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (tbMeta._indexes[i]._columnID == columnID && tbMeta._indexes[i]._exist)
        {
            tbMeta._indexes[i]._ofPK = true;
            tbMeta._primaryKey = priKey;
            if (!callByCreateTb)
                printf("Add primary key well.\n");
            _writeBackDBMeta();
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return true;
        }
    // add index for primary key
    for (int i = 0; i <= MAX_INDEX_NUM - 1; i++)
        if (!tbMeta._indexes[i]._exist)
        {
            Index index;
            index._exist = true;
            index._ofPK = true;
            index._columnID = priKey._columnID;
            // printf("index %d on %d column %s\n", i, index._columnID, attrName);
            priKey._indexID = i;
            _im->createIndexFile(tbMeta._name, i,
                                 tbMeta._columns[index._columnID]._attrType,
                                 tbMeta._columns[index._columnID]._attrLen);
            IndexHandler idxHandler;
            _im->openIndexFile(tbMeta._name, i, idxHandler);
            RecordHandler recHandler;
            _rm->openRecFile(NameUtil::recFileName(_DBName.c_str(), tableName).c_str(), recHandler);
            RecordScanner recScanner;
            int attrOffset = tbMeta.nullByteLen();
            for (int i = 0; i <= tbMeta._columnNum - 1; i++)
            {
                if (i == index._columnID)
                    break;
                attrOffset += tbMeta._columns[i]._attrLen;
            }
            recScanner.openScan(&recHandler);
            RID rid(-1, -1);
            Byte *recData = new Byte[recHandler._fHeader._slotLen];
            while (recScanner.getNextRecord(rid, recData))
                idxHandler.insertEntry(rid, recData + attrOffset);
            delete[] recData;
            recScanner.closeScan();
            _im->closeIndexFile(idxHandler);
            _rm->closeRecFile(recHandler);
            tbMeta._primaryKey = priKey;
            tbMeta._indexes[i] = index;
            if (!callByCreateTb)
                printf("Add primary key well.\n");
            _writeBackDBMeta();
            printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            return true;
        }
    printf("Can't create more index.\n");
    printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    return false;
}

bool SystemManager::dropPrimaryKey(const char *tableName)
{
    int tbID = _getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        return false;
    }
    TableMeta &tbMeta = _DBMeta._tbMetas[tbID];
    if (!tbMeta._primaryKey._exist)
    {
        printf("No primary key in this table.\n");
        return false;
    }
    for (int i = 0; i <= MAX_REFER_NUM - 1; i++)
        if (tbMeta._primaryKey._refers[i]._priTbID != -1)
        {
            printf("Foreign key on this primary key.\n");
            return false;
        }
    _im->destroyIndexFile(tbMeta._name, tbMeta._primaryKey._indexID);
    memset(&tbMeta._primaryKey, 0, sizeof(PrimaryKey));
    memset(&tbMeta._indexes[tbMeta._primaryKey._indexID], 0, sizeof(Index));
    _writeBackDBMeta();
    printf("Drop primary key well.\n");
    return true;
}

bool SystemManager::addForeignKey(const char *name,
                                  const char *forTbName, const char *forAttr,
                                  const char *priTbName, const char *priAttr,
                                  bool callByCreateTb)
{
    int priTbID = _getTableID(priTbName);
    if (priTbID < 0) // table not existing
    {
        printf("No table with this primary key.\n");
        return false;
    }
    int priColumnID = _getColumnID(priTbID, priAttr);
    if (priColumnID < 0) // column not existing
    {
        printf("This column to be primary key doesn't exist.\n");
        return false;
    }
    if (_DBMeta._tbMetas[priTbID]._columns[priColumnID]._canBeNull) // can't be primary key
    {
        printf("This column can't be primary key.\n");
        return false;
    }
    int forTbID = _getTableID(forTbName);
    if (forTbID < 0) // table not existing
    {
        printf("No table with this foreign key.\n");
        return false;
    }
    int forColumnID = _getColumnID(forTbID, forAttr);
    if (forColumnID < 0) // column not existing
    {
        printf("This column to be foreign key doesn't exist.\n");
        return false;
    }
    ForeignKey forKey;
    Reference refer;
    NameUtil::cpy(forKey._name, name);
    refer._priTbID = forKey._priTbID = priTbID;
    refer._priColumnID = forKey._priColumnID = priColumnID;
    refer._forTbID = forKey._forTbID = forTbID;
    refer._forColumnID = forKey._forColumnID = forColumnID;
    for (int i = 0; i <= MAX_REFER_NUM - 1; i++)
        if (_DBMeta._tbMetas[priTbID]._primaryKey._refers[i]._priTbID == -1)
        {
            for (int j = 0; j <= MAX_FOREIGN_KEY_NUM - 1; j++)
                if (_DBMeta._tbMetas[forTbID]._foreignKeys[j]._priTbID == -1)
                {
                    _DBMeta._tbMetas[priTbID]._primaryKey._refers[i] = refer;
                    _DBMeta._tbMetas[forTbID]._foreignKeys[j] = forKey;
                    if (!callByCreateTb)
                        printf("Add foreign key well.\n");
                    _writeBackDBMeta();
                    return true;
                }
            printf("Foreign keys are full.\n");
            return false;
        }
    printf("References on this primary key are full.\n");
    return false;
}

bool SystemManager::dropForeignKey(const char *tableName, const char *forKeyName)
{
    int tbID = _getTableID(tableName);
    if (tbID < 0) // table not existing
    {
        printf("Non-existing table.\n");
        return false;
    }
    for (int i = 0; i <= MAX_FOREIGN_KEY_NUM - 1; i++)
        if (NameUtil::cmp(forKeyName, _DBMeta._tbMetas[tbID]._foreignKeys[i]._name) == 0)
        {
            int priTbID = _DBMeta._tbMetas[tbID]._foreignKeys[i]._priTbID;
            for (int j = 0; j <= MAX_REFER_NUM - 1; j++)
            {
                int priColumnID = _getColumnID(priTbID, forKeyName);
                if (priColumnID == _DBMeta._tbMetas[priTbID]._primaryKey._refers[j]._priColumnID)
                {
                    memset(&_DBMeta._tbMetas[priTbID]._primaryKey._refers[j], 0, sizeof(Reference));
                    _DBMeta._tbMetas[priTbID]._primaryKey._refers[j]._priTbID = -1;
                    memset(&_DBMeta._tbMetas[tbID]._foreignKeys[i], 0, sizeof(ForeignKey));
                    _DBMeta._tbMetas[tbID]._foreignKeys[i]._priTbID = -1;
                    _writeBackDBMeta();
                    printf("Drop foreign key well.\n");
                    return true;
                }
            }
        }
    printf("Non-existing foreign key.\n");
    return false;
}
