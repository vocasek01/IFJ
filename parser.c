#include "parser.h"
#include "codegen.c"
#include "codegen.h"
#include "symtable.c"
#include "stack.c"
// #include "symtable.c"

Token token;
BSTNodePtr *symtable;
BSTNodePtr *root_symtable;
tokenStack tmp;
int counter_param = 0;
int returnCode;
typeVar change_type();
typeVar check_type();
char *char_type();
    // Load next token, check the return code.

    /*********TABLE*************/

    int start()
{
    // rule <start> -> <preamble> <first_body>
    NEXT();
    CHECK_AND_CALL_FUNCTION(preamble());
    
    generate_header();
    // NEXT();
    CHECK_AND_CALL_FUNCTION(firstBody());

    return OK;
}

int preamble()
{
    // rule <preamble> -> require "ifj21"

    checkAndLoadKeyword(KEYWORD, "require");

    if (token.type != IFJ21)
        return LEX_ERROR;

    NEXT();

    return OK;
}

int firstBody()
{
    smInit(&symtable);
    switch (token.type)
    {
    case KEYWORD:
        // Rule: <first_body> -> <func> <body>
        if (strcmp(token.attribute, "function") == 0)
        {
            CHECK_AND_CALL_FUNCTION(func());

            CHECK_AND_CALL_FUNCTION(body());
            return OK;
            break;
        } // Rule: <first_body> -> <func_declr> <body>
        else if (strcmp(token.attribute, "global") == 0)
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
        if (strcmp(token.attribute, "function") == 0)
        {
            CHECK_AND_CALL_FUNCTION(func());

            CHECK_AND_CALL_FUNCTION(body());
            return OK;
            break;
        } // Rule: <first_body> -> <func_declr> <body>
        else if (strcmp(token.attribute, "global") == 0)
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
        // printf();
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
    checkInsertAndLoadToken(KEYWORD, "function");
    smInsertFunctin(&symtable, token.attribute, NO); // symtable added name_func
    root_symtable = symtable;
    generate_func_top(token.attribute);
    checkAndLoadToken(IDENTIFICATOR);
    checkAndLoadToken(LBR);
    CHECK_AND_CALL_FUNCTION(params());
    checkAndLoadToken(RBR);
    CHECK_AND_CALL_FUNCTION(funcTypes());
    // generate_func_bottom(token);
    return OK;
}

int funcDeclr()
{
    // Rule: <func_declr>  ->  global id : function ( <params> ) : <types>
    checkInsertAndLoadToken(KEYWORD, "global");
    checkAndLoadToken(IDENTIFICATOR);
    checkAndLoadToken(COLON);
    checkAndLoadKeyword(KEYWORD, "function");
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
        counter_param++;
        generate_func_param(token.attribute, counter_param);
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
        checkAndLoadKeyword(KEYWORD, "end");
        return OK;
        break;
    case IDENTIFICATOR: // Rule: <func_types> ->  <state_list> end
        CHECK_AND_CALL_FUNCTION(stateList());
        checkAndLoadKeyword(KEYWORD, "end");
        return OK;
        break;
    case KEYWORD:
        if (strcmp(token.attribute, "end") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "local") == 0 
        || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "while") == 0)
        {
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, "end");
            return OK;
            break;
        }
        else
        {
            return SYNTAX_ERROR;
            break;
        }
    case EOL:
        NEXT();
        return OK; ////skip


    default : return SYNTAX_ERROR;
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
        if (strcmp(token.attribute, "function") == 0 || strcmp(token.attribute, "global") == 0)
        {
            return OK;
            break;
        } // Rule: <types> ->  <data_type>
        else if (strcmp(token.attribute, "integer") == 0 || strcmp(token.attribute, "nil") == 0 || strcmp(token.attribute, "string") == 0 || strcmp(token.attribute, "number") == 0)
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
        if (strcmp(token.attribute, "end") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "while") == 0)
        {
            return OK;
            break;
        } // Rule: <types> ->  <data_type>
        else if (strcmp(token.attribute, "integer") == 0 || strcmp(token.attribute, "nil") == 0 || strcmp(token.attribute, "string") == 0 || strcmp(token.attribute, "number") == 0)
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
        if (strcmp(token.attribute, "end") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "while") == 0)
        {
            return OK;
            break;
        } // Rule: <types> ->  <data_type>
        else if (strcmp(token.attribute, "integer") == 0 || strcmp(token.attribute, "nil") == 0 || strcmp(token.attribute, "string") == 0 || strcmp(token.attribute, "number") == 0)
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
        if (strcmp(token.attribute, "else") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "while") == 0)
        {
            return OK;
            break;
        } // Rule: <types> ->  <data_type>
        else if (strcmp(token.attribute, "integer") == 0 || strcmp(token.attribute, "nil") == 0 || strcmp(token.attribute, "string") == 0 || strcmp(token.attribute, "number") == 0)
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
    // generate_func_param(); ///need symtable_func Parametr
    return OK;
}

int paramsN()
{
    switch (token.type)
    {
    // Rule: <params_n> ->  eps
    case RBR:
        counter_param = 0;
        return OK;
        break;
    case COMMA: // Rule: <params_n> ->  , <data_type> <types_n>
        checkAndLoadToken(COMMA);
        // NEXT();
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
        if (strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "while") == 0)
        {
            stackInit(&tmp);
            CHECK_AND_CALL_FUNCTION(state());
            // NEXT();///added
            token = getToken();
            CHECK_AND_CALL_FUNCTION(stateList());
            return OK;
            break;
        } // Rule: <state_list>->eps
        else if (strcmp(token.attribute, "end") == 0)
        {
            symtable = root_symtable;
            generate_func_bottom(symtable->name);

            smDeleteFunction(&symtable);
            // smDeleteFunction(&root_symtable);
            // stackClear(&tmp);
            // stackFree(&tmp);
            return SYNTAX_ERROR;
            return OK;

            break;
        } // Rule: <state_list>->return <return>
        else if (strcmp(token.attribute, "return") == 0)
        {
            checkAndLoadKeyword(KEYWORD, "return");
            CHECK_AND_CALL_FUNCTION(returnState());
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
        if (strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "while") == 0)
        {
            CHECK_AND_CALL_FUNCTION(stateT35());
            CHECK_AND_CALL_FUNCTION(stateListT24());
            return OK;
            break;
        } // Rule: <state_list>->eps
        else if (strcmp(token.attribute, "else") == 0)
        {
            return OK;
            break;
        } // Rule: <state_list>->return <return>
        else if (strcmp(token.attribute, "return") == 0)
        {
            checkAndLoadKeyword(KEYWORD, "return");
            CHECK_AND_CALL_FUNCTION(returnStateT36());
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

int dataType()
{ /* Rule: <data_type> ->  integer
<data_type> ->  string
<data_type> ->  nil
<data_type> ->  number
*/
    switch (token.type)
    {
    case KEYWORD:
        if (strcmp(token.attribute, "integer") == 0)
        {
            ////adde type var in symtable*****
            if (symtable->isFunction != true)
            {
                symtable = smSearchNode(symtable, tmp.head.attribute);
                typeVar a = change_type();
                smInsertVariable(&symtable, tmp.head.attribute, token.attribute, a);
            }

            checkAndLoadKeyword(KEYWORD, "integer"); // добавить в стак или симтейбл
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "string") == 0)
        {
            checkAndLoadKeyword(KEYWORD, "string");
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "nil") == 0)
        {
            checkAndLoadKeyword(KEYWORD, "nil");
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "number") == 0)
        {
            checkAndLoadKeyword(KEYWORD, "number");
            return OK;
            break;
        }
        return SYNTAX_ERROR;
        break;
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
        if (strcmp(token.attribute, "function") == 0 || strcmp(token.attribute, "global") == 0)
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
        return OK;
        break;
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
        if (strcmp(token.attribute, "while") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "end") == 0)
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
        return OK;
        break;
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
        if (strcmp(token.attribute, "while") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "end") == 0)
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
        return OK;
        break;
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
        if (strcmp(token.attribute, "while") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "else") == 0)
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
        return OK;
        break;
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
        if (strcmp(token.attribute, "local") == 0) // Rule: <state>  ->  local id : <types> <is_assign>
        {
            checkAndLoadKeyword(KEYWORD, "local");
            stackPush(&tmp, token); ///added stack name var token 

            smInsertVariable(&symtable, token.attribute, NULL, NO); /// added symtable name var
            symtable = smSearchNode(symtable, tmp.head.attribute);

            // generate_declaration("LF@", token.attribute); /// added codegen
            checkAndLoadToken(IDENTIFICATOR);
            checkAndLoadToken(COLON);
            CHECK_AND_CALL_FUNCTION(typesT20()); //symtable *** добавить в симтейбл енам и добавить тип данных
            CHECK_AND_CALL_FUNCTION(isAssign()); //=
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "if") == 0) // Rule: <state> ->  if <expression> then <state_list> <after_if>
        {
            checkInsertAndLoadToken(KEYWORD, "if");
            CHECK_AND_CALL_FUNCTION(expr());
            checkAndLoadKeyword(KEYWORD, "then");
            CHECK_AND_CALL_FUNCTION(stateListT24());
            checkAndLoadKeyword(KEYWORD, "else");
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, "end");
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "while") == 0) // Rule: <state> ->  while <expression> do <state_list> end
        {
            checkInsertAndLoadToken(KEYWORD, "while");
            CHECK_AND_CALL_FUNCTION(expr());
            checkAndLoadKeyword(KEYWORD, "do");
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, "end");
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
        if (strcmp(token.attribute, "local") == 0) // Rule: <state>  ->  local id : <types> <is_assign>
        {
            checkAndLoadKeyword(KEYWORD, "local");
            checkAndLoadToken(IDENTIFICATOR);
            checkAndLoadToken(COLON);
            CHECK_AND_CALL_FUNCTION(typesT43());
            CHECK_AND_CALL_FUNCTION(isAssignT44());
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "if") == 0) // Rule: <state> ->  if <expression> then <state_list> <after_if>
        {
            checkInsertAndLoadToken(KEYWORD, "if");
            CHECK_AND_CALL_FUNCTION(expr());
            checkAndLoadKeyword(KEYWORD, "then");
            CHECK_AND_CALL_FUNCTION(stateListT24());
            checkAndLoadKeyword(KEYWORD, "else");
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, "end");
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "while") == 0) // Rule: <state> ->  while <expression> do <state_list> end
        {
            checkInsertAndLoadToken(KEYWORD, "while");
            CHECK_AND_CALL_FUNCTION(expr());
            checkAndLoadKeyword(KEYWORD, "do");
            CHECK_AND_CALL_FUNCTION(stateList());
            checkAndLoadKeyword(KEYWORD, "end");
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
        if (token.type == COMMA || token.type == LBR || strcmp(token.attribute, "end") == 0)
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
        if (strcmp(token.attribute, "end") == 0) // Rule: <return>  ->  eps
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
        if (token.type == COMMA || token.type == LBR || strcmp(token.attribute, "else") == 0)
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
        if (strcmp(token.attribute, "else") == 0) // Rule: <return>  ->  eps
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
        if (strcmp(token.attribute, "while") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "end") == 0) // Rule: <return>  ->  eps
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
        checkAndLoadToken(ASSIGN); //generate_move(char *dest_frame, char *identifier, char *source_frame, char *source)
        typeVar a =  check_type();
        smInsertVariable(&symtable, tmp.head.attribute, token.attribute, a);
        // stackPop(&tmp);
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
        if (strcmp(token.attribute, "while") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "else") == 0) // Rule: <return>  ->  eps
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
        CHECK_AND_CALL_FUNCTION(exprFuncT52());
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
        if (strcmp(token.attribute, "end") == 0) // Rule: <ret_type>  ->  <id_n>
        {
            CHECK_AND_CALL_FUNCTION(idNT37());
            return OK;
            break;
        }
        return SYNTAX_ERROR;
        break;
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
        if (strcmp(token.attribute, "else") == 0) // Rule: <ret_type>  ->  <id_n>
        {
            CHECK_AND_CALL_FUNCTION(idNT53());
            return OK;
            break;
        }
        return SYNTAX_ERROR;
        break;
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
    case STR: ///added
        symtable = smSearchNode(symtable, tmp.head.attribute);
        generate_declaration("LF@", symtable->name); ///added
        generate_move("LF@", symtable->name, char_type(), symtable->data); ///added codegen move
        stackPop(&tmp);
        return OK;
        break;
    // case DOUB_DOT1:
    // case DOUB_DOT2:
    // case DOUB_EXP1:
    // case DOUB_EXP2:
        // CHECK_AND_CALL_FUNCTION(expr());
        // return OK;
        // break;
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
        if (strcmp(token.attribute, "end") == 0) // Rule: <expr_n>  ->  eps
        {
            return OK;
            break;
        }
        return SYNTAX_ERROR;
        break;
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
        if (strcmp(token.attribute, "while") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "end") == 0) // Rule: <expr_n>  ->  eps
        {
            return OK;
            break;
        }
        return SYNTAX_ERROR;
        break;
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
        CHECK_AND_CALL_FUNCTION(exprNT48());
        return OK;
        break;
    case KEYWORD:
        if (strcmp(token.attribute, "else") == 0) // Rule: <expr_n>  ->  eps
        {
            return OK;
            break;
        }
        return SYNTAX_ERROR;
        break;
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
        if (strcmp(token.attribute, "while") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "else") == 0) // Rule: <expr_n>  ->  eps
        {
            return OK;
            break;
        }
        return SYNTAX_ERROR;
        break;
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
        if (strcmp(token.attribute, "end") == 0) // Rule: <id_n>  ->  eps
        {
            return OK;
            break;
        }
        return SYNTAX_ERROR;
        break;
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
        if (strcmp(token.attribute, "else") == 0) // Rule: <id_n>  ->  eps
        {
            return OK;
            break;
        }
        return SYNTAX_ERROR;
        break;
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
        return OK;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
}

/**
 * changes the variable type for symtable
 * takes token.attribute at the moment
 **/
typeVar change_type()
{
    if (strcmp(token.attribute, "integer") == 0)
    {
        return integer;
    }
    // else if (strcmp(token.attribute, "") == 0)
    // {
    //     return iteger;
    // }
    return NO;
}
/**
 * changes the variable type for symtable
 * takes token.attribute at the moment
 **/
typeVar check_type()
{
    if(token.type == INT && symtable->type == integer)
    {
        return integer;
    }

    return SYNTAX_ERROR;
}

char *char_type()
{
    if (symtable->type == integer)
    {
        return "int@";
    }
}
