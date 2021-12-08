/*------------------------------------------
 Title:  parser.h
 Author: Matěj Krátký, Koval Maksym, Kirillov Artem
 Login:  xkratk17, xkoval20, xkiril01
 Date:   23 Nov 2021;
------------------------------------------*/

#include "expression.h"
#pragma once


Token token;
int returnCode;

// Call function FUN and check return code.
#define CHECK_AND_CALL_FUNCTION(FUN) \
    {                                \
        returnCode = FUN;            \
        if (returnCode != OK)        \
        {                            \
            return returnCode;       \
        }                            \
    }

// Load next token, skip what is not needed.

#define NEXT()                                                                                                         \
    {                                                                                                                  \
        while (1)                                                                                                      \
        {                                                                                                              \
            token = getToken();                                                                                        \
            if (token.type != BLOCKORLINE && token.type != BLOCKCOMMENT && token.type != COMMENT && token.type != EOL) \
            {                                                                                                          \
                if (token.type == ERROR)                                                                               \
                {                                                                                                      \
                    return LEX_ERROR;                                                                                  \
                }                                                                                                      \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
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
    {                                                                      \
        if (token.type != TYPE || strcmp(token.attribute, ATTRIBUTE) != 0) \
        {                                                                  \
            return SYNTAX_ERROR;                                           \
        } /*insert into tree*/                                             \
        NEXT();                                                            \
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
int decType();
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
int funcParam();
int funcParamN();
typeVar change_enum(TokenType token);
typeVar change_type(char *type);
int chek_name(char *name);
int build_func(char *name);
int check_dec(char *name, int a);
int check_type();
char *nil(typeVar type);
char *char_type(typeVar type);
