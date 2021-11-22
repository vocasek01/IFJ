#include "parser.h"

Token token;
int returnCode;
// Load next token, check the return code.

void next()
{
    while (1)
    {
        token = getToken();
        if (token.type == ERROR)
        {
            return LEX_ERROR;
        }
        break;
    }
}

// Compare actual token with TOK and ATR, then call next token.
void checkAndLoadToken(TokenType type)
{
    if (token.type != type)
    {
        return SYNTAX_ERROR;
    }
    next();
}

void checkAndLoadKeyword(TokenType type, char *attribute)
{
    if (token.type != type || token.attribute != attribute)
    {
        return SYNTAX_ERROR;
    }
    next();
}

// Compare actual token with TOK and ATR, then insert into tree and call next token.
void checkInsertAndLoadToken(TokenType type, char *attribute)
{
    if (token.type != type || token.attribute != attribute)
    {
        return SYNTAX_ERROR;
    }
    /*insert into tree*/
    next();
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

    checkAndLoadKeyword(KEYWORD, 'package');

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
    checkInsertAndLoadToken(KEYWORD, 'function');
    checkAndLoadToken(IDENTIFICATOR);
    checkAndLoadToken(LBR);
    CHECK_AND_CALL_FUNCTION(params());
    checkAndLoadToken(RBR);
    CHECK_AND_CALL_FUNCTION(funcTypes());

    return OK;
}

int funcDeclr()
{
    // Rule: <func_declr>  ->  global id : function ( <params> ) : <types>
    checkInsertAndLoadToken(KEYWORD, 'global');
    checkAndLoadToken(IDENTIFICATOR);
    checkAndLoadToken(COLON);
    checkAndLoadKeyword(KEYWORD, 'function');
    checkAndLoadToken(LBR);
    CHECK_AND_CALL_FUNCTION(params());
    checkAndLoadToken(RBR);
    checkAndLoadToken(COLON);
    CHECK_AND_CALL_FUNCTION(types());

    return OK;
}

int funcCall()
{
    // Rule: <func_call> -> id ( <params> )
    checkAndLoadToken(IDENTIFICATOR);
    checkAndLoadToken(LBR);
    CHECK_AND_CALL_FUNCTION(params());
    checkAndLoadToken(RBR);

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
        checkAndLoadToken(COLON);
        CHECK_AND_CALL_FUNCTION(typesT11());
        CHECK_AND_CALL_FUNCTION(stateList());
        checkAndLoadKeyword(KEYWORD, 'end');
        return OK;
        break;
    case IDENTIFICATOR: // Rule: <func_types> ->  <state_list> end
        CHECK_AND_CALL_FUNCTION(stateList());
        checkAndLoadKeyword(KEYWORD, 'end');
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end' || token.attribute == 'return' || token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while')
        {
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, 'end');
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
int typesT43()
{
    switch (token.type)
    {
    // Rule: <types> ->  eps
    case ASSIGN:
        return OK;
        break;
    case IDENTIFICATOR:
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'else' || token.attribute == 'if' || token.attribute == 'return' || token.attribute == 'local' || token.attribute == 'while')
        {
            return OK;
            break;
        } // Rule: <types> ->  <data_type>
        else if (token.attribute == 'integer' || token.attribute == 'nil' || token.attribute == 'string' || token.attribute == 'number')
        {
            CHECK_AND_CALL_FUNCTION(dataType());
            CHECK_AND_CALL_FUNCTION(typesNT50());
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
    checkAndLoadToken(IDENTIFICATOR);
    checkAndLoadToken(COLON);
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
        checkAndLoadToken(COMMA);
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
            checkAndLoadKeyword(KEYWORD, 'return');
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
            checkAndLoadKeyword(KEYWORD, 'return');
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
            checkAndLoadKeyword(KEYWORD, 'integer');
            return OK;
            break;
        }
        else if (token.attribute == 'string')
        {
            checkAndLoadKeyword(KEYWORD, 'string');
            return OK;
            break;
        }
        else if (token.attribute == 'nil')
        {
            checkAndLoadKeyword(KEYWORD, 'nil');
            return OK;
            break;
        }
        else if (token.attribute == 'number')
        {
            checkAndLoadKeyword(KEYWORD, 'number');
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
        checkAndLoadToken(COMMA);
        CHECK_AND_CALL_FUNCTION(dataType());
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
        checkAndLoadToken(COMMA);
        CHECK_AND_CALL_FUNCTION(dataType());
        CHECK_AND_CALL_FUNCTION(typesNT17());
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int typesNT30()
{
    switch (token.type)
    {
        // Rule: <types_n>   ->  eps
    case IDENTIFICATOR:
        return OK;
        break;
    case ASSIGN:
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
        checkAndLoadToken(COMMA);
        CHECK_AND_CALL_FUNCTION(dataType());
        CHECK_AND_CALL_FUNCTION(typesNT30());
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int typesNT50()
{
    switch (token.type)
    {
        // Rule: <types_n>   ->  eps
    case IDENTIFICATOR:
        return OK;
        break;
    case ASSIGN:
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'while' || token.attribute == 'if' || token.attribute == 'local' || token.attribute == 'return' || token.attribute == 'else')
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
        checkAndLoadToken(COMMA);
        CHECK_AND_CALL_FUNCTION(dataType());
        CHECK_AND_CALL_FUNCTION(typesNT50());
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
        checkAndLoadToken(IDENTIFICATOR);
        CHECK_AND_CALL_FUNCTION(afterID());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'local') // Rule: <state>  ->  local id : <types> <is_assign>
        {
            checkAndLoadKeyword(KEYWORD, 'local');
            checkAndLoadToken(IDENTIFICATOR);
            checkAndLoadToken(COLON);
            CHECK_AND_CALL_FUNCTION(typesT20());
            CHECK_AND_CALL_FUNCTION(isAssign());
            return OK;
            break;
        }
        else if (token.attribute == 'if') // Rule: <state> ->  if <expression> then <state_list> <after_if>
        {
            checkInsertAndLoadToken(KEYWORD, 'if');
            CHECK_AND_CALL_FUNCTION(expr());
            checkAndLoadKeyword(KEYWORD, 'then');
            CHECK_AND_CALL_FUNCTION(stateListT24());
            checkAndLoadKeyword(KEYWORD, 'else');
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, 'end');
            return OK;
            break;
        }
        else if (token.attribute == 'while') // Rule: <state> ->  while <expression> do <state_list> end
        {
            checkInsertAndLoadToken(KEYWORD, 'while');
            CHECK_AND_CALL_FUNCTION(expr());
            checkAndLoadKeyword(KEYWORD, 'do');
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, 'end');
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

int stateT35()
{
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <state> ->  id <after_id>
        checkAndLoadToken(IDENTIFICATOR);
        CHECK_AND_CALL_FUNCTION(afterIDT45());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'local') // Rule: <state>  ->  local id : <types> <is_assign>
        {
            checkAndLoadKeyword(KEYWORD, 'local');
            checkAndLoadToken(IDENTIFICATOR);
            checkAndLoadToken(COLON);
            CHECK_AND_CALL_FUNCTION(typesT43());
            CHECK_AND_CALL_FUNCTION(isAssignT44());
            return OK;
            break;
        }
        else if (token.attribute == 'if') // Rule: <state> ->  if <expression> then <state_list> <after_if>
        {
            checkInsertAndLoadToken(KEYWORD, 'if');
            CHECK_AND_CALL_FUNCTION(expr());
            checkAndLoadKeyword(KEYWORD, 'then');
            CHECK_AND_CALL_FUNCTION(stateListT24());
            checkAndLoadKeyword(KEYWORD, 'else');
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, 'end');
            return OK;
            break;
        }
        else if (token.attribute == 'while') // Rule: <state> ->  while <expression> do <state_list> end
        {
            checkInsertAndLoadToken(KEYWORD, 'while');
            CHECK_AND_CALL_FUNCTION(expr());
            checkAndLoadKeyword(KEYWORD, 'do');
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, 'end');
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
        checkAndLoadToken(IDENTIFICATOR);
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
    case INT:
    case DOUBLE:
    case DOUB_DOT1:
    case DOUB_DOT2:
    case DOUB_EXP1:
    case DOUB_EXP2:
        CHECK_AND_CALL_FUNCTION(expr());
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
int returnStateT36()
{
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <return> ->  id <ret_type>
        checkAndLoadToken(IDENTIFICATOR);
        if (token.type == COMMA || token.type == LBR || token.attribute == 'else')
        {
            CHECK_AND_CALL_FUNCTION(retTypeT46());
        }
        else
        { // Rule: <return> ->  <expression>
            CHECK_AND_CALL_FUNCTION(expr());
            CHECK_AND_CALL_FUNCTION(exprNT48());
        }
        return OK;
        break;
    case INT:
    case DOUBLE:
    case DOUB_DOT1:
    case DOUB_DOT2:
    case DOUB_EXP1:
    case DOUB_EXP2:
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprNT48());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'else') // Rule: <return>  ->  eps
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
        checkAndLoadToken(ASSIGN);
        CHECK_AND_CALL_FUNCTION(declr());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int isAssignT44()
{
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <is_assign> ->  eps
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'else' || token.attribute == 'return' || token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while') // Rule: <return>  ->  eps
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
        checkAndLoadToken(ASSIGN);
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

        checkAndLoadToken(LBR);
        CHECK_AND_CALL_FUNCTION(funcParam());
        checkAndLoadToken(RBR);
        return OK;
        break;
    case COMMA: // Rule:  <after_id> -> <id_n> = <expr_func>
        CHECK_AND_CALL_FUNCTION(idN());
        checkAndLoadToken(ASSIGN);
        CHECK_AND_CALL_FUNCTION(exprFunc());
        return OK;
        break;
    case ASSIGN:
        CHECK_AND_CALL_FUNCTION(idN());
        checkAndLoadToken(ASSIGN);
        CHECK_AND_CALL_FUNCTION(exprFunc());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int afterIDT45()
{
    switch (token.type)
    {
    case LBR: // Rule:<after_id> -> ( <func_param> )

        checkAndLoadToken(LBR);
        CHECK_AND_CALL_FUNCTION(funcParam());
        checkAndLoadToken(RBR);
        return OK;
        break;
    case COMMA: // Rule:  <after_id> -> <id_n> = <expr_func>
        CHECK_AND_CALL_FUNCTION(idN());
        checkAndLoadToken(ASSIGN);
        CHECK_AND_CALL_FUNCTION(exprFuncT52());
        return OK;
        break;
    case ASSIGN:
        CHECK_AND_CALL_FUNCTION(idN());
        checkAndLoadToken(ASSIGN);
        CHECK_AND_CALL_FUNCTION(exprFuncT52()); ///if expr_func is similar, this is not required
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
        checkAndLoadToken(LBR);
        CHECK_AND_CALL_FUNCTION(funcParam());
        checkAndLoadToken(RBR);
        return OK;
        break;
    case COMMA: // Rule: <ret_type>  ->  <id_n>
        CHECK_AND_CALL_FUNCTION(idNT37());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end') // Rule: <ret_type>  ->  <id_n>
        {
            CHECK_AND_CALL_FUNCTION(idNT37());
            return OK;
            break;
        }
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int retTypeT46()
{
    switch (token.type)
    {
    case LBR: // Rule: <ret_type> -> ( <func_param> )
        checkAndLoadToken(LBR);
        CHECK_AND_CALL_FUNCTION(funcParam());
        checkAndLoadToken(RBR);
        return OK;
        break;
    case COMMA: // Rule: <ret_type>  ->  <id_n>
        CHECK_AND_CALL_FUNCTION(idNT53());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'else') // Rule: <ret_type>  ->  <id_n>
        {
            CHECK_AND_CALL_FUNCTION(idNT53());
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
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <declr>     ->  id ( <func_param> )
        checkAndLoadToken(IDENTIFICATOR);
        if (token.type == LBR)
        {
            checkAndLoadToken(LBR);
            CHECK_AND_CALL_FUNCTION(funcParam());
            checkAndLoadToken(RBR);
        }
        else // Rule: <declr>     ->  <expression>
        {
            CHECK_AND_CALL_FUNCTION(expr());
        }
        return OK;
        break;
    case INT:
    case DOUBLE:
    case DOUB_DOT1:
    case DOUB_DOT2:
    case DOUB_EXP1:
    case DOUB_EXP2:
        CHECK_AND_CALL_FUNCTION(expr());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int exprN()
{
    switch (token.type)
    {
    case COMMA: // Rule: <expr_n>  ->  , <expr> <expr_n>
        checkAndLoadToken(COMMA);
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

int exprNT40()
{
    switch (token.type)
    {
    case COMMA: // Rule: <expr_n>  ->  , <expr> <expr_n>
        checkAndLoadToken(COMMA);
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprNT40());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end' || token.attribute == 'return' || token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while') // Rule: <expr_n>  ->  eps
        {
            return OK;
            break;
        }
    case IDENTIFICATOR:
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int exprNT48()
{
    switch (token.type)
    {
    case COMMA: // Rule: <expr_n>  ->  , <expr> <expr_n>
        checkAndLoadToken(COMMA);
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprN48());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'else') // Rule: <expr_n>  ->  eps
        {
            return OK;
            break;
        }
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int exprNT56()
{
    switch (token.type)
    {
    case COMMA: // Rule: <expr_n>  ->  , <expr> <expr_n>
        checkAndLoadToken(COMMA);
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprNT56());
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'else' || token.attribute == 'return' || token.attribute == 'local' || token.attribute == 'if' || token.attribute == 'while') // Rule: <expr_n>  ->  eps
        {
            return OK;
            break;
        }
    case IDENTIFICATOR:
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int idN()
{
    switch (token.type)
    {
    case COMMA: // Rule: <id_n>  ->  , id <id_n>
        checkAndLoadToken(COMMA);
        checkAndLoadToken(IDENTIFICATOR);
        CHECK_AND_CALL_FUNCTION(idN());
        return OK;
        break;
    case ASSIGN: // Rule: <id_n>  ->  eps
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int idNT37()
{
    switch (token.type)
    {
    case COMMA: // Rule: <id_n>  ->  , id <id_n>
        checkAndLoadToken(COMMA);
        checkAndLoadToken(IDENTIFICATOR);
        CHECK_AND_CALL_FUNCTION(idNT37());
        return OK;
        break;
    case ASSIGN: // Rule: <id_n>  ->  eps
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'end') // Rule: <id_n>  ->  eps
        {
            return OK;
            break;
        }
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int idNT53()
{
    switch (token.type)
    {
    case COMMA: // Rule: <id_n>  ->  , id <id_n>
        checkAndLoadToken(COMMA);
        checkAndLoadToken(IDENTIFICATOR);
        CHECK_AND_CALL_FUNCTION(idNT53());
        return OK;
        break;
    case ASSIGN: // Rule: <id_n>  ->  eps
        return OK;
        break;
    case KEYWORD:
        if (token.attribute == 'else') // Rule: <id_n>  ->  eps
        {
            return OK;
            break;
        }
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int exprFunc()
{
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <expr_func> ->  id ( <func_param> )
        checkAndLoadToken(IDENTIFICATOR);
        if (token.type == LBR)
        {
            checkAndLoadToken(LBR);
            CHECK_AND_CALL_FUNCTION(funcParam());
            checkAndLoadToken(RBR);
        }
        else // Rule: <expr_func> ->  <expression> <expression_n>
        {
            CHECK_AND_CALL_FUNCTION(expr());
            CHECK_AND_CALL_FUNCTION(exprNT40());
        }
        return OK;
        break;
    case INT:
    case DOUBLE:
    case DOUB_DOT1:
    case DOUB_DOT2:
    case DOUB_EXP1:
    case DOUB_EXP2:
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprNT40());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}
int exprFuncT52()
{
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <expr_func> ->  id ( <func_param> )
        checkAndLoadToken(IDENTIFICATOR);
        if (token.type == LBR)
        {
            checkAndLoadToken(LBR);
            CHECK_AND_CALL_FUNCTION(funcParam());
            checkAndLoadToken(RBR);
        }
        else // Rule: <expr_func> ->  <expression> <expression_n>
        {
            CHECK_AND_CALL_FUNCTION(expr());
            CHECK_AND_CALL_FUNCTION(exprNT56());
        }
        return OK;
        break;
    case INT:
    case DOUBLE:
    case DOUB_DOT1:
    case DOUB_DOT2:
    case DOUB_EXP1:
    case DOUB_EXP2:
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprNT56());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int funcParam()
{
    switch (token.type)
    {
    case RBR:
        return OK;
        break;
    case IDENTIFICATOR: // Rule: <func_param>     ->  <expression> <func_param_n>
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(funcParamN());
        return OK;
        break;
    case INT:
    case DOUBLE:
    case DOUB_DOT1:
    case DOUB_DOT2:
    case DOUB_EXP1:
    case DOUB_EXP2:
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(funcParamN());
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

int funcParamN()
{
    switch (token.type)
    { // Rule: <func_param_n>     -> eps
    case RBR:
        return OK;
        break;
    case COMMA: // Rule: <func_param_n>    ->  , <expression> <func_param_n>
        checkAndLoadToken(COMMA);
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(funcParamN());
    default:
        return SYNTAX_ERROR;
        break;
    }
}
