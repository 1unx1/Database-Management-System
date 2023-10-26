%code requires {
}

%{
#include <cstdio>
#include <vector>
#include <ctime>
#include "../defines.h"
#include "../SystemManager/SystemManager.h"
#include "../QueryManager/QueryManager.h"
#include "../IndexManager/IndexManager.h"
#include "../RecordManager/RecordManager.h"
extern int yylex(void);
void yyerror(const char *s, ...);
%}

%union{
    char *str;
    Column *column;
    PK *pk;
    FK *fk;
    std::vector<FK> *fkList;
    ColumnType type;
    bool boolean;
    std::vector<Column> *columnList;
    std::vector<Expr> *exprList;
    std::vector<std::vector<Expr>> *valLists;
    Expr *expr;
    CmpOp op;
}

%token <str> IDENTIFIER STR_INT STR_FLOAT STR_VARCHAR
%token SAFE QUIT CREATE DROP SHOW USE DESC INSERT DELETE UPDATE SELECT ALTER ADD
%token INTO VALUES FROM WHERE SET NOT DEFAULT AND
%token DATABASES DATABASE TABLES TABLE INDEX PRIMARY FOREIGN KEY CONSTRAINT REFERENCES
%token T_INT T_FLOAT T_VARCHAR
%token STR_NULL

%type <type> attr_type
%type <boolean> not_null
%type <column> field
%type <pk> pk
%type <fk> fk
%type <fkList> fk_list
%type <valLists> value_lists
%type <columnList> field_list
%type <exprList> where_and_clause cond_list set_list column_list selectors value_list values
%type <expr> cond_expr set_expr col_expr val_expr default
%type <op> cmp_op assign


%%
program 
    : statements
    ;

statements 
    : statements statement
    | statement
    ;

statement
    : db_statement
    | table_statement
    | alter_statement
    | QUIT ';'
    {
		YYACCEPT;
    }
    ;

db_statement
    : CREATE DATABASE IDENTIFIER ';'
    {
        sm->createDB($3);
    }
    | DROP DATABASE IDENTIFIER ';'
    {
        sm->dropDB($3);
    }
    | SHOW DATABASES ';'
    {
        sm->showDBs();
    }
    | USE IDENTIFIER ';'
    {
        sm->useDB($2);
    }
    | SHOW TABLES ';'
    {
        sm->showTables();
    }
    | SHOW USE ';'
    {
        sm->showUse();
    }
    ;

table_statement
    : CREATE TABLE IDENTIFIER '(' field_list pk fk_list ')' ';'
    {
        sm->createTable($3, $5);
        delete $5;
        sm->addPrimaryKey($3, $6->_columnName, true);
        delete $6;
        for (FK &fk : *$7)
            sm->addForeignKey(fk._priColName, $3, fk._forColName, fk._priTbName, fk._priColName, true);
        delete $7;
    }
    | CREATE TABLE IDENTIFIER '(' field_list pk ')' ';'
    {
        sm->createTable($3, $5);
        delete $5;
        sm->addPrimaryKey($3, $6->_columnName, true);
        delete $6;
    }
    | CREATE TABLE IDENTIFIER '(' field_list fk_list ')' ';'
    {
        sm->createTable($3, $5);
        delete $5;
        for (FK &fk : *$6)
            sm->addForeignKey(fk._priColName, $3, fk._forColName, fk._priTbName, fk._priColName, true);
        delete $6;
    }
    | CREATE TABLE IDENTIFIER '(' field_list ')' ';'
    {
        sm->createTable($3, $5);
        delete $5;
    }
    | DROP TABLE IDENTIFIER ';'
    {
        sm->dropTable($3);
    }
    | DESC IDENTIFIER ';'
    {
        sm->descTable($2);
    }
    | SAFE INSERT INTO IDENTIFIER VALUES value_lists ';'
    {
        qm->safeInsert($4, $6);
        delete $6;
    }
    | INSERT INTO IDENTIFIER VALUES value_lists ';'
    {
        clock_t start = clock();
        int success = 0;
        for (std::vector<Expr> values : *$5){
            if (qm->insertRec($3, &values))
                success++;
        }
        printf("Successfully insert %d record of %d.\n", success, (int)($5->size()));
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        delete $5;
    }
    | DELETE FROM IDENTIFIER where_and_clause ';'
    {
        qm->deleteRec($3, $4);
        if ($4 != nullptr)
            delete $4;
    }
    | UPDATE IDENTIFIER SET set_list where_and_clause ';'
    {
        qm->updateRec($2, $4, $5);
        delete $4;
        if ($5 != nullptr)
            delete $5;
    }
    | SELECT selectors FROM IDENTIFIER where_and_clause ';'
    {
        qm->selectRec(false, $4, nullptr, $2, $5);
        if ($2 != nullptr)
            delete $2;
        if ($5 != nullptr)
            delete $5;
    }
    | SELECT selectors FROM IDENTIFIER ',' IDENTIFIER where_and_clause ';'
    {
        qm->selectRec(true, $4, $6, $2, $7);
        if ($2 != nullptr)
            delete $2;
        if ($7 != nullptr)
            delete $7;
    }

alter_statement
    : ALTER TABLE IDENTIFIER ADD INDEX '(' IDENTIFIER ')' ';'
    {
        sm->addIndex($3, $7);
    }
    | ALTER TABLE IDENTIFIER DROP INDEX '(' IDENTIFIER ')' ';'
    {
        sm->dropIndex($3, $7);
    }
    | ALTER TABLE IDENTIFIER DROP PRIMARY KEY ';'
    {
        sm->dropPrimaryKey($3);
    }
    | ALTER TABLE IDENTIFIER DROP FOREIGN KEY IDENTIFIER ';'
    {
        sm->dropForeignKey($3, $7);
    }
    | ALTER TABLE IDENTIFIER ADD CONSTRAINT PRIMARY KEY '(' IDENTIFIER ')' ';'
    {
        sm->addPrimaryKey($3, $9);
    }
    | ALTER TABLE IDENTIFIER ADD CONSTRAINT FOREIGN KEY '(' IDENTIFIER ')' REFERENCES IDENTIFIER '(' IDENTIFIER ')' ';'
    {
        sm->addForeignKey($14, $3, $9, $12, $14);
    }
    ;

// create table
field_list
    : field_list ',' field
    {
        $$ = $1;
        $$->push_back(*$3);
        delete $3;

    }
    | field
    {
        $$ = new std::vector<Column>;
        $$->push_back(*$1);
        delete $1;
    }
    ;

field
    : IDENTIFIER attr_type not_null default
    {
        $$ = new Column;
        NameUtil::cpy($$->_name, $1);
        $$->_attrType = $2;
        $$->_attrLen = 4;
        $$->_canBeNull = !$3;
    }
    | IDENTIFIER T_VARCHAR '(' STR_INT ')' not_null default
    {
        $$ = new Column;
        NameUtil::cpy($$->_name, $1);
        $$->_attrType = VARCHAR;
        $$->_attrLen = atoi($4);
        $$->_canBeNull = !$6;
    }
    ;

pk
    : ',' PRIMARY KEY '(' IDENTIFIER ')'
    {
        $$ = new PK;
        NameUtil::cpy($$->_columnName, $5);
    }
    ;

fk_list
    : fk_list ',' fk
    {
        $$ = $1;
        $$->push_back(*$3);
        delete $3;
    }
    | ',' fk
    {
        $$ = new std::vector<FK>;
        $$->push_back(*$2);
        delete $2;
    }
    ;

fk
    : FOREIGN KEY '(' IDENTIFIER ')' REFERENCES IDENTIFIER '(' IDENTIFIER ')'
    {
        $$ = new FK;
        NameUtil::cpy($$->_forColName, $4);
        NameUtil::cpy($$->_priTbName, $7);
        NameUtil::cpy($$->_priColName, $9);
    }
    ;

attr_type
    : T_INT
    {
        $$ = INT;
    }
    | T_FLOAT
    {
        $$ = FLOAT;
    }
    ;

not_null
    :
    {
        $$ = false;
    }
    | NOT STR_NULL
    {
        $$ = true;
    }
    ;

default
    :
    {
        $$ = nullptr;
    }
    | DEFAULT val_expr
    {
        $$ = $2;
    }
    ;

// insert
value_lists
    : value_lists ',' value_list
    {
        $$ = $1;
        $$->push_back(*$3);
        delete $3;
    }
    | value_list
    {
        $$ = new std::vector<std::vector<Expr>>;
        $$->push_back(*$1);
        delete $1;
    }
    ;

value_list
    : '(' values ')'
    {
        $$ = $2;
    }
    ;

values
    : values ',' val_expr
    {
        $$ = $1;
        $$->push_back(*$3);
        delete $3;
    }
    | val_expr
    {
        $$ = new std::vector<Expr>;
        $$->push_back(*$1);
        delete $1;
    }
    ;

// where clause
where_and_clause
    :
    {
        $$ = nullptr;
    }
    | WHERE cond_list
    {
        $$ = $2;
    }
cond_list
    : cond_list AND cond_expr
    {
        $$ = $1;
        $$->push_back(*$3);
        delete $3;
    }
    | cond_expr
    {
        $$ = new std::vector<Expr>;
        $$->push_back(*$1);
        delete $1;
    }
    ;

cond_expr
    : col_expr cmp_op val_expr
    {
        $$ = new Expr($1, $3, $2);
    }
    | col_expr cmp_op col_expr
    {
        $$ = new Expr($1, $3, $2);
    }
    ;

col_expr
    : IDENTIFIER
    {
        $$ = new Expr($1, nullptr);
    }
    | IDENTIFIER '.' IDENTIFIER
    {
        $$ = new Expr($3, $1);
    }

val_expr
    : STR_INT
    {
        $$ = new Expr(atoi($1));
    }
    | STR_FLOAT
    {
        $$ = new Expr((float)(atof($1)));
    }
    | STR_VARCHAR
    {
        $$ = new Expr($1, 200);
    }
    | STR_NULL
    {
        $$ = new Expr(true);
    }
    ;

cmp_op
    : '<'
    {
        $$ = LT;
    }
    | '<' '='
    {
        $$ = LE;
    }
    | '>'
    {
        $$ = GT;
    }
    | '>' '='
    {
        $$ = GE;
    }
    | '='
    {
        $$ = EQ;
    }
    | '<' '>'
    {
        $$ = NE;
    }
    ;

// set clause
set_list
    : set_list ',' set_expr
    {
        $$ = $1;
        $$->push_back(*$3);
        delete $3;
    }
    | set_expr
    {
        $$ = new std::vector<Expr>;
        $$->push_back(*$1);
        delete $1;
    }
    ;

set_expr
    : col_expr assign val_expr
    {
        $$ = new Expr($1, $3, $2);
    }
    ;

assign
    : '='
    {
        $$ = EQ;
    }

// select
selectors
    : '*'
    {
        $$ = nullptr;
    }
    | column_list
    {
        $$ = $1;
    }
    ;

column_list
    : column_list ',' col_expr
    {
        $$ = $1;
        $$->push_back(*$3);
        delete $3;
    }
    | col_expr
    {
        $$ = new std::vector<Expr>;
        $$->push_back(*$1);
        delete $1;
    }
    ;
%%

void yyerror(const char *s)
{
    printf("Error: %s\n", s);
}
