#include "parser.h"

Token token;
int returnCode;
// Load next token, check the return code.
#define NEXT()                        \
    {                                 \
        while (1)                     \
        {                             \
            &token = getToken();      \
            if (token.type == ERROR)  \
            {                         \
                return LEXICAL_ERROR; \
            }                         \
            break;                    \
        }                             \
    }

// Compare actual token with TOK and ATR, then call next token.
#define CHECK_AND_LOAD_TOKEN(TYPE) \
    {                              \
        if (token.type != TYPE)    \
        {                          \
            return SYNTAX_ERROR;   \
        }                          \
        NEXT();                    \
    }

#define CHECK_AND_LOAD_KEYWORD(TYPE, ATR)                 \
    {                                                     \
        if (token.type != TYPE || token.attribute != ATR) \
        {                                                 \
            return SYNTAX_ERROR;                          \
        }                                                 \
        NEXT();                                           \
    }
// Compare actual token with TOK and ATR, then insert into tree and call next token.
#define CHECK_INSERT_AND_LOAD_TOKEN(TYPE, ATR)            \
    {                                                     \
        if (token.type != TYPE || token.attribute != ATR) \
        {                                                 \
            return SYNTAX_ERROR;                          \
        }                                                 \
        /*insert into tree*/                              \
        NEXT();                                           \
    }

// Call function FUN and check return code.
#define CHECK_AND_CALL_FUNCTION(FUN) \
    {                                \
        returnCode = FUN;            \
        if (returnCode != OK)        \
        {                            \
            return returnCode;       \
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
int declr();
int exprN();
int idN();
int exprFunc();
int funcParam();
int funcParamN();

int start()
{
    // rule <start> -> <preamble> <first_body>
    CHECK_AND_CALL_FUNCTION(preamble());

    CHECK_AND_CALL_FUNCTION(firstBody());

    return OK;
}

int preamble()
{
    // rule <preamble> -> require "ifj21"

    CHECK_AND_LOAD_KEYWORD(KEYWORD, 'package');

    if (token.type == STR)
    {
        char *requireName = '"ifj21"';
        if (token.attribute != requireName)
        {
            return LEX_ERROR;
        }
    }
    else
        return SYNTAX_ERROR;

    NEXT();

    return OK;
}

int firstBody()
{
    switch (token.type)
    {
    case KEYWORD:
        // Rule: <first_body> -> <func> <body>
        if (token.attribute == 'function')
        {
            CHECK_AND_CALL_FUNCTION(func());

            CHECK_AND_CALL_FUNCTION(body());
            return OK;
            break;
        } // Rule: <first_body> -> <func_declr> <body>
        else if (token.attribute == 'global')
        {
            CHECK_AND_CALL_FUNCTION(funcDeclr());

            CHECK_AND_CALL_FUNCTION(body());
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }
    case IDENTIFICATOR:
        // Rule: <first_body> -> <func_call> <body>
        CHECK_AND_CALL_FUNCTION(funcCall());

        CHECK_AND_CALL_FUNCTION(body());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int body()
{
    switch (token.type)
    {
    case KEYWORD:
        // Rule: <first_body> -> <func> <body>
        if (token.attribute == 'function')
        {
            CHECK_AND_CALL_FUNCTION(func());

            CHECK_AND_CALL_FUNCTION(body());
            return OK;
            break;
        } // Rule: <first_body> -> <func_declr> <body>
        else if (token.attribute == 'global')
        {
            CHECK_AND_CALL_FUNCTION(funcDeclr());

            CHECK_AND_CALL_FUNCTION(body());
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }
    case IDENTIFICATOR:
        // Rule: <first_body> -> <func_call> <body>
        CHECK_AND_CALL_FUNCTION(funcCall());

        CHECK_AND_CALL_FUNCTION(body());
        return OK;
        break;
    case ENDOFFILE:
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int func()
{
    // Rule: <func> -> function id ( <params> ) <func_types>
    CHECK_INSERT_AND_LOAD_TOKEN(KEYWORD, 'function');
    CHECK_AND_LOAD_TOKEN(IDENTIFICATOR);
    CHECK_AND_LOAD_TOKEN(LBR);
    CHECK_AND_CALL_FUNCTION(params());
    CHECK_AND_LOAD_TOKEN(RBR);
    CHECK_AND_CALL_FUNCTION(funcTypes());

    return OK;
}

int funcDeclr()
{
    // Rule: <func_declr>  ->  global id : function ( <params> ) : <types>
    CHECK_INSERT_AND_LOAD_TOKEN(KEYWORD, 'global');
    CHECK_AND_LOAD_TOKEN(IDENTIFICATOR);
    CHECK_AND_LOAD_TOKEN(COLON);
    CHECK_AND_LOAD_KEYWORD(KEYWORD, 'function');
    CHECK_AND_LOAD_TOKEN(LBR);
    CHECK_AND_CALL_FUNCTION(params());
    CHECK_AND_LOAD_TOKEN(RBR);
    CHECK_AND_LOAD_TOKEN(COLON);
    CHECK_AND_CALL_FUNCTION(types());

    return OK;
}

int funcCall()
{
    // Rule: <func_call> -> id ( <params> )
    CHECK_INSERT_AND_LOAD_TOKEN(IDENTIFICATOR);
    CHECK_AND_LOAD_TOKEN(LBR);
    CHECK_AND_CALL_FUNCTION(params());
    CHECK_AND_LOAD_TOKEN(RBR);

    return OK;
}

int params()
{
    switch (token.type)
    {
    case RBR:
        return OK;
        break;
    case IDENTIFICATOR:
        // Rule: <first_body> -> <func_call> <body>
        CHECK_AND_CALL_FUNCTION(param());
        CHECK_AND_CALL_FUNCTION(paramsN());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int funcTypes()
{
    switch (token.type)
    {
    case COLON: // Rule: <func_types> -> : <types> <state_list> end
        CHECK_AND_LOAD_TOKEN(COLON);
        CHECK_AND_CALL_FUNCTION(typesT11());
        CHECK_AND_CALL_FUNCTION(stateList());
        CHECK_AND_LOAD_KEYWORD(KEYWORD, 'end');
        return OK;
        break;
    case IDENTIFICATOR: // Rule: <func_types> ->  <state_list> end
        CHECK_AND_CALL_FUNCTION(stateList());
        CHECK_AND_LOAD_KEYWORD(KEYWORD, 'end');
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end' || token.attribute == 'return' || token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while')
        {
            CHECK_AND_CALL_FUNCTION(stateList());
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'end');
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int types()
{
    switch (token.type)
    {
    // Rule: <types> ->  eps
    case ENDOFFILE:
        return OK;
        break;
    case IDENTIFICATOR:
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'function' || token.attribute == 'global')
        {
            return OK;
            break;
        } // Rule: <types> ->  <data_type>
        else if (token.attribute == 'integer' || token.attribute == 'nil' || token.attribute == 'string' || token.attribute == 'number')
        {
            CHECK_AND_CALL_FUNCTION(dataType());
            CHECK_AND_CALL_FUNCTION(typesN());
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }

    default:
        return SYNTAX_ERROR;
        break;
    }
}
int typesT11()
{
    switch (token.type)
    {
    // Rule: <types> ->  eps
    case IDENTIFICATOR:
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end' || token.attribute == 'return' || token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while')
        {
            return OK;
            break;
        } // Rule: <types> ->  <data_type>
        else if (token.attribute == 'integer' || token.attribute == 'nil' || token.attribute == 'string' || token.attribute == 'number')
        {
            CHECK_AND_CALL_FUNCTION(dataType());
            CHECK_AND_CALL_FUNCTION(typesNT17());
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }

    default:
        return SYNTAX_ERROR;
        break;
    }
}
int typesT20()
{
    switch (token.type)
    {
    // Rule: <types> ->  eps
    case IDENTIFICATOR:
        return OK;
        break;
    case ASSIGN:
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end' || token.attribute == 'return' || token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while')
        {
            return OK;
            break;
        } // Rule: <types> ->  <data_type>
        else if (token.attribute == 'integer' || token.attribute == 'nil' || token.attribute == 'string' || token.attribute == 'number')
        {
            CHECK_AND_CALL_FUNCTION(dataType());
            CHECK_AND_CALL_FUNCTION(typesNT30());
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }

    default:
        return SYNTAX_ERROR;
        break;
    }
}

int param()
{ // Rule:<param>     ->  id : <data_type>
    CHECK_AND_LOAD_TOKEN(IDENTIFICATOR);
    CHECK_AND_LOAD_TOKEN(COLON);
    CHECK_AND_CALL_FUNCTION(dataType());
    return OK;
}

int paramsN()
{
    switch (token.type)
    {
    // Rule: <params_n> ->  eps
    case RBR:
        return OK;
        break;
    case COMMA: // Rule: <params_n> ->  , <data_type> <types_n>
        CHECK_AND_LOAD_TOKEN(COMMA);
        CHECK_AND_CALL_FUNCTION(param());
        CHECK_AND_CALL_FUNCTION(paramsN());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int stateList()
{
    switch (token.type)
    {
    // Rule:<state_list> ->  <state> <state_list>
    case IDENTIFICATOR:
        CHECK_AND_CALL_FUNCTION(state());
        CHECK_AND_CALL_FUNCTION(stateList());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while')
        {
            CHECK_AND_CALL_FUNCTION(state());
            CHECK_AND_CALL_FUNCTION(stateList());
            return OK;
            break;
        } // Rule: <state_list>->eps
        else if (token.attribute == 'end')
        {
            return OK;
            break;
        } // Rule: <state_list>->return <return>
        else if (token.attribute == 'return')
        {
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'return');
            CHECK_AND_CALL_FUNCTION(returnState());
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }

    default:
        return SYNTAX_ERROR;
        break;
    }
}

int stateListT24()
{
    switch (token.type)
    {
    // Rule:<state_list> ->  <state> <state_list>
    case IDENTIFICATOR:
        CHECK_AND_CALL_FUNCTION(stateT35());
        CHECK_AND_CALL_FUNCTION(stateListT24());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while')
        {
            CHECK_AND_CALL_FUNCTION(stateT35());
            CHECK_AND_CALL_FUNCTION(stateListT24());
            return OK;
            break;
        } // Rule: <state_list>->eps
        else if (token.attribute == 'else')
        {
            return OK;
            break;
        } // Rule: <state_list>->return <return>
        else if (token.attribute == 'return')
        {
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'return');
            CHECK_AND_CALL_FUNCTION(returnStateT36());
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }

    default:
        return SYNTAX_ERROR;
        break;
    }
}

int dataType()
{ /* Rule: <data_type> ->  integer
<data_type> ->  string
<data_type> ->  nil
<data_type> ->  number
*/
    switch (token.type)
    {
    case KEYWORD:
        if (token.attribute == 'integer')
        {
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'integer');
            return OK;
            break;
        }
        else if (token.attribute == 'string')
        {
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'string');
            return OK;
            break;
        }
        else if (token.attribute == 'nil')
        {
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'nil');
            return OK;
            break;
        }
        else if (token.attribute == 'number')
        {
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'number');
            return OK;
            break;
        }
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int typesN()
{
    switch (token.type)
    {
        // Rule: <types_n>   ->  eps
    case IDENTIFICATOR:
        return OK;
        break;
    case ENDOFFILE:
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'function' || token.attribute == 'global')
        {
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }
    case COMMA: // Rule: <types_n> ->  , <data_type> <types_n>
        CHECK_AND_LOAD_TOKEN(COMMA);
        CHECK_AND_CALL_FUNCTION(types());
        CHECK_AND_CALL_FUNCTION(typesN());
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int typesNT17()
{
    switch (token.type)
    {
        // Rule: <types_n>   ->  eps
    case IDENTIFICATOR:
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'while' || token.attribute == 'if' || token.attribute == 'local' || token.attribute == 'return' || token.attribute == 'end')
        {
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }
    case COMMA: // Rule: <types_n> ->  , <data_type> <types_n>
        CHECK_AND_LOAD_TOKEN(COMMA);
        CHECK_AND_CALL_FUNCTION(types());
        CHECK_AND_CALL_FUNCTION(typesNT17());
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int typesNT30() /*rule 30 end continue here*/
{
    switch (token.type)
    {
        // Rule: <types_n>   ->  eps
    case IDENTIFICATOR:
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'while' || token.attribute == 'if' || token.attribute == 'local' || token.attribute == 'return' || token.attribute == 'end')
        {
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }
    case COMMA: // Rule: <types_n> ->  , <data_type> <types_n>
        CHECK_AND_LOAD_TOKEN(COMMA);
        CHECK_AND_CALL_FUNCTION(types());
        CHECK_AND_CALL_FUNCTION(typesNT17());
    default:
        return SYNTAX_ERROR;
        break;
    }
}


int state()
{
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <state> ->  id <after_id>
        CHECK_AND_LOAD_TOKEN(IDENTIFICATOR);
        CHECK_AND_CALL_FUNCTION(afterID());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'local') // Rule: <state>  ->  local id : <types> <is_assign>
        {
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'local');
            CHECK_AND_LOAD_TOKEN(IDENTIFICATOR);
            CHECK_AND_LOAD_TOKEN(COLON);
            CHECK_AND_CALL_FUNCTION(typesT20());
            CHECK_AND_CALL_FUNCTION(isAssign());
            return OK;
            break;
        }
        else if (token.attribute == 'if') // Rule: <state> ->  if <expression> then <state_list> <after_if>
        {
            CHECK_INSERT_AND_LOAD_TOKEN(KEYWORD, 'if');
            CHECK_AND_CALL_FUNCTION(expr());
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'then');
            CHECK_AND_CALL_FUNCTION(stateListT24());
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'else');
            CHECK_AND_CALL_FUNCTION(stateList());
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'end');
            return OK;
            break;
        }
        else if (token.attribute == 'while') // Rule: <state> ->  while <expression> do <state_list> end
        {
            CHECK_INSERT_AND_LOAD_TOKEN(KEYWORD, 'while');
            CHECK_AND_CALL_FUNCTION(expr());
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'do');
            CHECK_AND_CALL_FUNCTION(stateList());
            CHECK_AND_LOAD_KEYWORD(KEYWORD, 'end');
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }

    default:
        return SYNTAX_ERROR;
        break;
    }
}

int returnState()
{
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <return> ->  id <ret_type>
        CHECK_AND_LOAD_TOKEN(IDENTIFICATOR);
        if (token.type == COMMA || token.type == LBR || token.attribute == 'end')
        {
            CHECK_AND_CALL_FUNCTION(retType());
        }
        else
        { // Rule: <return> ->  <expression>
            CHECK_AND_CALL_FUNCTION(expr());
        }
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end') // Rule: <return>  ->  eps
        {
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }

    default:
        return SYNTAX_ERROR;
        break;
    }
}

int isAssign()
{
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <is_assign> ->  eps

        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end' || token.attribute == 'return' || token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while') // Rule: <return>  ->  eps
        {
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }
    case ASSIGN: // Rule: <is_assign> ->  = <declr>
        CHECK_AND_LOAD_TOKEN(ASSIGN);
        CHECK_AND_CALL_FUNCTION(declr());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int afterID()
{
    switch (token.type)
    {
    case LBR: // Rule:<after_id> -> ( <func_param> )

        CHECK_AND_LOAD_TOKEN(LBR);
        CHECK_AND_CALL_FUNCTION(funcParam());
        CHECK_AND_LOAD_TOKEN(RBR);
        return OK;
        break;
    case COMMA: // Rule:  <after_id> -> <id_n> = <expr_func>
        CHECK_AND_CALL_FUNCTION(idN());
        CHECK_AND_LOAD_TOKEN(ASSIGN);
        CHECK_AND_CALL_FUNCTION(exprFunc());
        return OK;
        break;
    case ASSIGN:
        CHECK_AND_CALL_FUNCTION(idN());
        CHECK_AND_LOAD_TOKEN(ASSIGN);
        CHECK_AND_CALL_FUNCTION(exprFunc());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int expr()
{
    return OK;
}

int retType()
{
    switch (token.type)
    {
    case LBR: // Rule: <ret_type> -> ( <func_param> )
        CHECK_AND_LOAD_TOKEN(LBR);
        CHECK_AND_CALL_FUNCTION(funcParam());
        CHECK_AND_LOAD_TOKEN(RBR);
        return OK;
        break;
    case COMMA: // Rule: <ret_type>  ->  <id_n>
        CHECK_AND_CALL_FUNCTION(idN37());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end') // Rule: <ret_type>  ->  <id_n>
        {
            CHECK_AND_CALL_FUNCTION(idN37());
            return OK;
            break;
        }
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int declr()
{
    return OK;
}

int exprN()
{
    switch (token.type)
    {
    case COMMA: // Rule: <expr_n>  ->  , <expr> <expr_n>
        CHECK_AND_LOAD_TOKEN(COMMA);
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprN());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end') // Rule: <expr_n>  ->  eps
        {
            return OK;
            break;
        }
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int idN()
{
    return OK;
}

int exprFunc()
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
