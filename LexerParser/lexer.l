%option noyywrap
%{
#include <cstdio>
#include <vector>
#include "../defines.h"
#include "../SystemManager/SystemManager.h"
#include "../QueryManager/QueryManager.h"
#include "../IndexManager/IndexManager.h"
#include "../RecordManager/RecordManager.h"
#include "parser.tab.h"
%}
%%
-?[0-9]+                    {
                            yylval.str = new char[strlen(yytext) + 1];
                            memcpy(yylval.str, yytext, strlen(yytext));
                            yylval.str[strlen(yytext)] = '\0';
                            return STR_INT;
                            }
-?[0-9]+\.[0-9]*            {
                            yylval.str = new char[strlen(yytext) + 1];
                            memcpy(yylval.str, yytext, strlen(yytext));
                            yylval.str[strlen(yytext)] = '\0';
                            return STR_FLOAT;
                            }
QUIT|quit                   return QUIT;
CREATE|create               return CREATE;
DROP|drop                   return DROP;
SHOW|show                   return SHOW;
USE|use                     return USE;
DESC|desc                   return DESC;
INSERT|insert               return INSERT;
DELETE|delete               return DELETE;
UPDATE|update               return UPDATE;
SELECT|select               return SELECT;
ALTER|alter                 return ALTER;
ADD|add                     return ADD;
SAFE|safe                   return SAFE;

INTO|into                   return INTO;
VALUES|values               return VALUES;
FROM|from                   return FROM;
WHERE|where                 return WHERE;
SET|set                     return SET;
NOT|not                     return NOT;
DEFAULT|default             return DEFAULT;
AND|and                     return AND;

DATABASES|databases         return DATABASES;
DATABASE|database           return DATABASE;
TABLES|tables               return TABLES;
TABLE|table                 return TABLE;
INDEX|index                 return INDEX;
PRIMARY|primary             return PRIMARY;
FOREIGN|foreign             return FOREIGN;
KEY|key                     return KEY;
CONSTRAINT|constraint       return CONSTRAINT;
REFERENCES|references       return REFERENCES;

INT|int                     return T_INT;
FLOAT|float                 return T_FLOAT;
VARCHAR|varchar             return T_VARCHAR;
NULL|null                   return STR_NULL;
[a-zA-Z_][a-zA-Z_0-9]*      {
                            yylval.str = new char[strlen(yytext) + 1];
                            memcpy(yylval.str, yytext, strlen(yytext));
                            yylval.str[strlen(yytext)] = '\0';
                            return IDENTIFIER;
                            }
'[^']*'                     {
                            yylval.str = new char[strlen(yytext) - 1];
                            memcpy(yylval.str, yytext + 1, strlen(yytext) - 2);
                            yylval.str[strlen(yytext) - 2] = '\0';
                            return STR_VARCHAR;
                            }
[*-=<>';,.()]        return yytext[0];
%%