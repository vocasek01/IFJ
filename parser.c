#include "parser.h"

Token token;

// Load next token, check the return code and check if EOL is allowed.
#define NEXT()                             \
    {                                      \
        while (1)                          \
        {                                  \
            &token = getToken();           \
            if (token.token_type == ERROR) \
            {                              \
                return LEXICAL_ERROR;      \
            }                              \
            break;                         \
        }                                  \
    }

// Compare actual token with TOK and then call next token.
#define CHECK_AND_LOAD_TOKEN(TOK)    \
    {                                \
        if (token.token_type != TOK) \
        {                            \
            return SYNTAX_ERROR;     \
        }                            \
        NEXT();                      \
    }

// Call function FUN and check return code.
#define CHECK_AND_CALL_FUNCTION(FUN) \
    {                                \
        return_code = FUN;           \
        if (return_code != OK)       \
        {                            \
            return return_code;      \
        }                            \
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
int param();
int paramsN();
int stateList();
int dataType();
int typesN();
int state();
int returnState();
int isAssign();
int afterID();
int expr();
int retType();
int exprN();
int idN();
int exprFunc();
int elseState();
int funcParam();
int funcParamN();

int start()
{
    return OK;
}

int preamble()
{
    return OK;
}

int firstBody()
{
    return OK;
}

int body()
{
    return OK;
}

int func()
{
    return OK;
}

int funcDeclr()
{
    return OK;
}

int funcCall()
{
    return OK;
}

int params()
{
    return OK;
}

int funcTypes()
{
    return OK;
}

int types()
{
    return OK;
}

int param()
{
    return OK;
}

int paramsN()
{
    return OK;
}

int stateList()
{
    return OK;
}

int dataType()
{
    return OK;
}

int typesN()
{
    return OK;
}

int state()
{
    return OK;
}

int returnState()
{
    return OK;
}

int isAssign()
{
    return OK;
}

int afterID()
{
    return OK;
}

int expr()
{
    return OK;
}

int retType()
{
    return OK;
}

int exprN()
{
    return OK;
}

int idN()
{
    return OK;
}

int exprFunc()
{
    return OK;
}

int elseState()
{
    return OK;
}

int funcParam()
{
    return OK;
}

int funcParamN()
{
    return OK;
}
