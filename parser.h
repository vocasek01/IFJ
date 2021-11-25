#pragma once
#include "scanner.h"
#include "symtable.h"
#include "codegen.h"


#define OK 0
#define BAD 1
#define LEX_ERROR 1
#define SYNTAX_ERROR 2

// Call function FUN and check return code.
#define CHECK_AND_CALL_FUNCTION(FUN) \
    {                                \
        returnCode = FUN;            \
        if (returnCode != OK)        \
        {                            \
            return returnCode;       \
        }                            \
    }

#define NEXT()                       \
    {                                \
        while (1)                    \
        {                            \
            token = getToken();      \
            if (token.type == ERROR) \
            {                        \
                return LEX_ERROR;    \
            }                        \
            break;                   \
        }                            \
    }

// Compare actual token with TOK and ATR, then call next token.
#define checkAndLoadToken(TYPE)  \
    {                            \
        if (token.type != TYPE)  \
        {                        \
            return SYNTAX_ERROR; \
        }                        \
        NEXT();                  \
    }

#define checkAndLoadKeyword(TYPE, ATTRIBUTE)                               \
    {                                                                      \
        if (token.type != TYPE || strcmp(token.attribute, ATTRIBUTE) != 0) \
        {                                                                  \
            return SYNTAX_ERROR;                                           \
        }                                                                  \
        NEXT();                                                            \
    }

// Compare actual token with TOK and ATR, then insert into tree and call next token.
#define checkInsertAndLoadToken(TYPE, ATTRIBUTE)                           \
    {                                                                         \
        if (token.type != TYPE || strcmp(token.attribute, ATTRIBUTE) != 0) \
        {                                                                     \
            return SYNTAX_ERROR;                                              \
        } /*insert into tree*/                                                \
        NEXT();                                                               \
    }

// Init nonterminal states:

int start();
int preamble();
int firstBody();
int body();
int func();
int funcDeclr();
int funcCall();
int params();
int funcTypes();
int types();
int typesT11();
int typesT20();
int typesT43();
int param();
int paramsN();
int stateList();
int stateListT24();
int dataType();
int typesN();
int typesNT17();
int typesNT30();
int typesNT50();
int state();
int stateT35();
int returnState();
int returnStateT36();
int isAssign();
int isAssignT44();
int afterID();
int afterIDT45();
int expr();
int retType();
int retTypeT46();
int declr();
int exprN();
int exprNT40();
int exprNT48();
int exprNT56();
int idN();
int idNT37();
int idNT53();
int exprFunc();
int exprFuncT52();
int funcParam();   //
int funcParamN(); //
