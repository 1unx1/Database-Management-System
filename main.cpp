#include "defines.h"
#include "SystemManager/SystemManager.h"
#include "QueryManager/QueryManager.h"
#include "IndexManager/IndexManager.h"
#include "RecordManager/RecordManager.h"
#include "FileSystem/bufmanager/BufPageManager.h"
#include "FileSystem/fileio/FileManager.h"
#include "FileSystem/utils/pagedef.h"
#include <iostream>
extern int yyparse(QueryManager *qm, SystemManager *sm);
void yyerror(const char *s);
extern FILE *yyin;
int main(int argc, char *argv[])
{
    MyBitMap::initConst();
    FileManager fm;
    BufPageManager bpm(&fm);
    RecordManager rm(&bpm, &fm);
    IndexManager im(&bpm, &fm);
    SystemManager sm(&bpm, &fm, &rm, &im);
    QueryManager qm(&sm);
    if (argc == 1)
        yyparse(&qm, &sm);
    else
    {
        yyin = fopen(argv[1], "r");
        if (yyin == nullptr)
        {
            printf("Can not open SQL file %s\n", argv[1]);
            return 0;
        }
        while (!feof(yyin))
            yyparse(&qm, &sm);
        fclose(yyin);
    }
    return 0;
}
