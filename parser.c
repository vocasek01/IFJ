#include "parser.h"
#include "codegen.c"
#include "codegen.h"
#include "symtable.c"
#include "stack.c"
#include "expression.c"

Token token;
BSTNodePtr *symtable;
BSTNodePtr *root_symtable;
// tokenStack tmp;
Token clipboard [16];
Token nameFunc [16];
Token callFunc;
int counter_func = -1;
int counter_if = -1;

int counter_type = 0;
int counter_retVal = 0;

int counter_param = 0;
int count_bracket = 0;
int returnCode;

Stack expressionStack;
    // Load next token, check the return code.

    /*********TABLE*************/

    int start()
{
    // rule <start> -> <preamble> <first_body>
    NEXT();
    CHECK_AND_CALL_FUNCTION(preamble());
    
    generate_header();
    stackInit(&expressionStack);
    // NEXT();
    CHECK_AND_CALL_FUNCTION(firstBody());

    smDispose(&root_symtable);
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
    symtable = root_symtable;
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
    counter_func++;
    nameFunc[counter_func] = token;
    smInsertFunctin(&symtable, token.attribute, NO, NULL, NO, 0, 0); // symtable added name_func
    root_symtable = symtable;
    generate_func_top(token.attribute);
    checkAndLoadToken(IDENTIFICATOR);
    checkAndLoadToken(LBR);
    CHECK_AND_CALL_FUNCTION(params());
    checkAndLoadToken(RBR);
    count_bracket = 2;
    CHECK_AND_CALL_FUNCTION(funcTypes());
    // generate_func_bottom(token);
    counter_type = 0;
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
        // counter_param++;
        // generate_func_param(token.attribute, counter_param);
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
        CHECK_AND_CALL_FUNCTION(stateList()); //++++++POSSIBLE ERROR++++
        symtable = root_symtable;
        checkAndLoadKeyword(KEYWORD, "end"); ///fixed after return +++++++++
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
            for (int i=0; symtable->type[i] != NO; i++)
            {
                generate_return_values(symtable->type[i], i);
            }
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
            if (strcmp(token.attribute, "=") != 0)
            {
                generate_declaration(char_type(symtable->scope), symtable->name);
            }
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

    counter_param++; ///for cod gen
    smInsertFunctin(&symtable, nameFunc[counter_func].attribute, NO, token.attribute, NO, counter_param-1, 0);
    generate_func_param(token.attribute, counter_param); /// for cod gen

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
            // stackInit(&tmp);
            CHECK_AND_CALL_FUNCTION(state());
            // NEXT();///added
            // token = getToken();
            CHECK_AND_CALL_FUNCTION(stateList());
            return OK;
            break;
        } // Rule: <state_list>->eps
        else if (strcmp(token.attribute, "end") == 0)
        {
            symtable = root_symtable;
            if (symtable->name != NULL)
            {
                if (symtable->name[0] == 'i' && symtable->name[1] == 'f')
                    return OK;
            }
            generate_func_bottom(symtable->name); ///FIX MY 

            // smDeleteFunction(&symtable);
            // smDeleteFunction(&root_symtable);
            // stackClear(&tmp);
            // stackFree(&tmp);
            return OK;

            break;
        } // Rule: <state_list>->return <return>
        else if (strcmp(token.attribute, "return") == 0)
        {
            checkAndLoadKeyword(KEYWORD, "return");
            symtable = root_symtable;
            smAddRetVal(&symtable, token.attribute, counter_retVal++);           //FIX MY create var_couter return variabls
            CHECK_AND_CALL_FUNCTION(returnState());
            for (int i=0; symtable->retVal[i].name != NULL; i++)
            {
                char a[10];
                sprintf(a, "retval%d", i);
                // generate_declaration("LF@", a);
                generate_move("LF@", a, "LF@", symtable->retVal[i].name); //FIX MY create scope
                // counter_retVal++;
            }

            generate_func_bottom(symtable->name);

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
            symtable = root_symtable;
            smAddRetVal(&symtable, token.attribute, 0);
            CHECK_AND_CALL_FUNCTION(returnStateT36()); //FIX MY++++++++++
            for (int i = 0; symtable->retVal[i].name != NULL; i++)
            {
                char a[10];
                sprintf(a, "retval%d", i);
                // generate_declaration("LF@", a);
                generate_move("LF@", a, "LF@", symtable->retVal[i].name); // FIX MY create scope
            }
            generate_func_bottom(symtable->name);
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
                symtable = smSearchNode(symtable, clipboard[1].attribute);
                // typeVar a = change_type(token.attribute);
                smInsertVariable(&symtable, clipboard[1].attribute, NULL, change_type(token.attribute), change_type(clipboard[0].attribute));
            }
            else if (symtable->isFunction == true)
            {
                if (count_bracket == 2)
                {
                    smInsertFunctin(&symtable, nameFunc[counter_func].attribute, change_type(token.attribute), symtable->param[0].name, symtable->param[0].type, -1, counter_type++); //FIX MY
                }
                else
                    smInsertFunctin(&symtable, nameFunc[counter_func].attribute, NO, symtable->param[counter_param - 1].name, change_type(token.attribute), counter_param - 1, 0);
            }
            /// create safe param in symtable

            checkAndLoadKeyword(KEYWORD, "integer"); // добавить в стак или симтейбл
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "string") == 0)
        {
            if (symtable->isFunction != true)
            {
                symtable = smSearchNode(symtable, clipboard[1].attribute);
                // typeVar a = change_type(token.attribute);
                smInsertVariable(&symtable, clipboard[1].attribute, NULL, change_type(token.attribute), change_type(clipboard[0].attribute));
            }
            else if (symtable->isFunction == true)
            {
                if (count_bracket == 2)
                {
                    smInsertFunctin(&symtable, nameFunc[counter_func].attribute, change_type(token.attribute), symtable->param[0].name, symtable->param[0].type, -1, counter_type++); // FIX MY
                }
                else
                    smInsertFunctin(&symtable, nameFunc[counter_func].attribute, NO, symtable->param[counter_param - 1].name, change_type(token.attribute), counter_param - 1, 0);
            }
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
        // call func or приправнивание переменной
        clipboard[0] = token;
        checkAndLoadToken(IDENTIFICATOR);
        CHECK_AND_CALL_FUNCTION(afterID());
        if (strcmp(clipboard[0].attribute, "write") == 0)
        {
            for (int i=0; expressionStack.head.attribute != NULL; i++)
            {
                generate_write("LF@", expressionStack.head.attribute);
                stackPop(&expressionStack);
            }
        }
        return OK;
        break;
    case KEYWORD:
        if (strcmp(token.attribute, "local") == 0) // Rule: <state>  ->  local id : <types> <is_assign>
        {
            clipboard[0] = token;
            checkAndLoadKeyword(KEYWORD, "local");
            clipboard[1] = token; ///added stack name var token

            smInsertVariable(&symtable, clipboard[1].attribute, NULL, NO, change_type(clipboard[0].attribute)); /// added symtable name var
            symtable = smSearchNode(symtable, clipboard[1].attribute);

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
            symtable = root_symtable;
            counter_if++;
            char name_if[6];
            sprintf(name_if,"if%i", counter_if);
            smInsertFunctin(&symtable, name_if, NO, NULL, NO, 0, 0);
            root_symtable = symtable;
            checkInsertAndLoadToken(KEYWORD, "if");

            CHECK_AND_CALL_FUNCTION(expr());
            generate_if_head(stackTop(&expressionStack).attribute, counter_if);
            stackPop(&expressionStack);
            checkAndLoadKeyword(KEYWORD, "then");
            CHECK_AND_CALL_FUNCTION(stateListT24());

            generate_if_middle(counter_if);
            checkAndLoadKeyword(KEYWORD, "else");
            CHECK_AND_CALL_FUNCTION(stateList());

            generate_if_end(counter_if);
            smDeleteFunction(&symtable);
            root_symtable = symtable;
            // counter_if--;
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
        clipboard[0] = token;
        checkAndLoadToken(IDENTIFICATOR);
        CHECK_AND_CALL_FUNCTION(afterIDT45());
        if (strcmp(clipboard[0].attribute, "write") == 0)
        {
            for (int i = 0; expressionStack.head.attribute != NULL; i++)
            {
                generate_write("LF@", expressionStack.head.attribute);
                stackPop(&expressionStack);
            }
        }
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
            symtable = root_symtable;
            counter_if++;
            char name_if[6];
            sprintf(name_if, "if%i", counter_if);
            smInsertFunctin(&symtable, name_if, NO, NULL, NO, 0, 0);
            root_symtable = symtable;
            checkInsertAndLoadToken(KEYWORD, "if");

            CHECK_AND_CALL_FUNCTION(expr());
            generate_if_head(stackTop(&expressionStack).attribute, counter_if);
            stackPop(&expressionStack);
            checkAndLoadKeyword(KEYWORD, "then");
            CHECK_AND_CALL_FUNCTION(stateListT24());

            generate_if_middle(counter_if);
            checkAndLoadKeyword(KEYWORD, "else");
            CHECK_AND_CALL_FUNCTION(stateList());

            generate_if_end(counter_if);
            smDeleteFunction(&symtable);
            root_symtable = symtable;
            // counter_if--;
            checkAndLoadKeyword(KEYWORD, "end");
            return OK;
            break;
            // checkInsertAndLoadToken(KEYWORD, "if");
            // CHECK_AND_CALL_FUNCTION(expr());
            // checkAndLoadKeyword(KEYWORD, "then");
            // CHECK_AND_CALL_FUNCTION(stateListT24());
            // checkAndLoadKeyword(KEYWORD, "else");
            // CHECK_AND_CALL_FUNCTION(stateList());
            // checkAndLoadKeyword(KEYWORD, "end");
            // return OK;
            // break;
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
        // typeVar a =  check_type();
        smInsertVariable(&symtable, clipboard[1].attribute, token.attribute, check_type(), change_type(clipboard[0].attribute));
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
        // creat frame
        //safe name func
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
    int stop_time = 0; //just for test DELETEME
    int par_number = 0;
    Stack tokenStack;
    Token tmp,a,b,shift;
    shift.attribute = NULL;
    shift.type = E_SHIFT;

    stackInit(&tokenStack);
    tmp.type = E_STOP;
    tmp.attribute = NULL;


    stackPush(&tokenStack,tmp);

    int result = 0;
    while (true) {

        stop_time++;
        if (token.type == ERROR) {
            return LEX_ERROR;
        }


        a = find_term(&tokenStack);

        if (token.type == KEYWORD || token.type == COMMA || token.type == RBR && par_number == 0) { // FIXME
            b.type = E_STOP;
        }else {
            b = token;        
        }

        if (b.type == E_STOP && find_term(&tokenStack).type == E_STOP) {    // expression stop
            break;
        }

        int rule = find_rule(a.type, b.type);       // precedence[a,b]
        if (rule == ERROR) return ERROR;

        switch (rule)
        {
        
        // equals case
        case E:
            stackPush(&tokenStack,b);
            
            if (b.type == LBR) par_number++;
            else if (b.type == RBR) par_number--;

            NEXT();
            break;

        // smaller case
        case S:
            tmp = stackTop(&tokenStack);
            int isShifted = 0;
            switch (tmp.type)
            {
                case E_STOP:
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                case INT_DIV:
                case LEN:
                case GT:
                case LT:
                case NOTEQ:
                case GTE:
                case LTE:
                case EQ:
                case KONC:
                    break;
                default:
                    isShifted = 1;
                    break;
            }

            if (isShifted) {
                stackPop(&tokenStack);
            }

            stackPush(&tokenStack,shift);

            if (b.type == LBR) par_number++;
            else if (b.type == RBR) par_number--;

            if (isShifted) {
                stackPush(&tokenStack,tmp);
            }
            stackPush(&tokenStack,b);
            NEXT();
            break;

        // larger case
        case L:

            convert_to_nonterm(root_symtable, &tokenStack);

            break;

        // error case
        case X:
            return ERROR;
            break;

        default:
            break;
        }


    }

    Token expression = stackTop(&tokenStack);
    stackPop(&tokenStack);
    stackPush(&expressionStack,expression);
    // stackFree(&tokenStack);
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
            generate_declaration(char_type(symtable->scope), symtable->name); // FIX MY carefully EROR
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
        symtable = root_symtable;
        symtable = smSearchNode(symtable, clipboard[1].attribute);
        generate_declaration(char_type(symtable->scope), symtable->name); ///added
        generate_move(char_type(symtable->scope), symtable->name, char_type(symtable->type[0 ]), symtable->data); ///added codegen move
        NEXT();
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
        if (strcmp(token.attribute, "while") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "local") == 0 || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "else") == 0 || strcmp(token.attribute, "end") == 0)// Rule: <expr_n>  ->  eps
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

        smAddRetVal(&symtable, token.attribute, counter_retVal++);
        checkAndLoadToken(IDENTIFICATOR);   //?????
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
        callFunc = token;
        // symtable = smSearchNode(root_symtable, token.attribute);
        if (smSearcParamFunc(root_symtable, token.attribute) == NULL )
        {
            if (smSearchNode(root_symtable, token.attribute)->isFunction == true)
                checkAndLoadToken(IDENTIFICATOR);  
        }

        if (token.type == LBR)
        {
            checkAndLoadToken(LBR);
            CHECK_AND_CALL_FUNCTION(funcParam());
            checkAndLoadToken(RBR);

            int countP = 0;
            char charParam[32];
            for(int i=0; symtable->param[i].name != NULL; i++)
            {
                sprintf(charParam, "%%%d", i+1);
                generate_declaration("TF@", charParam);
                // symtable = smSearchNode(root_symtable, expressionStack.head.attribute);
                generate_move("TF@", charParam, char_type(change_enum(expressionStack.head.type)), expressionStack.head.attribute);
                // symtable = smSearchNode(root_symtable, callFunc.attribute);
                stackPop(&expressionStack);
            }

            generate_function_call(callFunc.attribute);

            symtable = smSearchNode(root_symtable, callFunc.attribute);
            for(int i=0; symtable->type[i] != NO; i++)
            {
                char charRetval[10];
                sprintf(charRetval, "retval%d", i);
                // symtable = smSearchNode(root_symtable, expressionStack.head.attribute);
                generate_move("LF@", clipboard[0].attribute,"TF@", charRetval);
                // symtable = smSearchNode(root_symtable, callFunc.attribute);
            }
            symtable = root_symtable;
            

            
        }
        else // Rule: <expr_func> ->  <expression> <expression_n>
        {
            CHECK_AND_CALL_FUNCTION(expr());
            CHECK_AND_CALL_FUNCTION(exprNT40());

            symtable = smSearchNode(root_symtable, clipboard[0].attribute);
            if (smSearcParamFunc(root_symtable, clipboard[0].attribute) == NULL)
            {
                if (symtable != NULL)
                {
                    generate_move(char_type(symtable->scope), symtable->name, char_type(change_enum(expressionStack.head.type)), expressionStack.head.attribute);
                    stackPop(&expressionStack);
                    return OK;
                }
                else
                    return SYNTAX_ERROR;
            }
            generate_move("LF@", smSearcParamFunc(root_symtable, clipboard[0].attribute)->name, char_type(change_enum(expressionStack.head.type)), expressionStack.head.attribute);
            stackPop(&expressionStack);
        }
        return OK;
        break;
    case INT:
    case STR:
    case DOUBLE:
    case DOUB_DOT1:
    case DOUB_DOT2:
    case DOUB_EXP1:
    case DOUB_EXP2:
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprNT56());

        if (symtable != NULL) //FIXED исправить поиск, добавить поиск параметров
        {
            for (int i=0; symtable->param[i].name != NULL; i++)
            {
                if (strcmp(symtable->param[i].name, clipboard[0].attribute) == 0)
                {
                    generate_move("LF@", clipboard[0].attribute, char_type(change_enum(expressionStack.head.type)), expressionStack.head.attribute);
                    return OK;
                }
            }
        }
        if (symtable != NULL)
        {
            generate_move(char_type(symtable->scope), clipboard[0].attribute, char_type(change_enum(expressionStack.head.type)), expressionStack.head.attribute);
        }else
            return SYNTAX_ERROR;

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
    case STR:
    case DOUBLE:
    case DOUB_DOT1:
    case DOUB_DOT2:
    case DOUB_EXP1:
    case DOUB_EXP2:
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprNT56()); //FIX MY как и вдругом такомже 
        symtable = smSearchNode(root_symtable, clipboard[0].attribute);
        if (symtable != NULL)
        {
            generate_move(char_type(symtable->scope), symtable->name, char_type(change_enum(expressionStack.head.type)), expressionStack.head.attribute);
        }
        else
            return SYNTAX_ERROR;

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
    case STR:
    case DOUBLE:
    case DOUB_DOT1:
    case DOUB_DOT2:
    case DOUB_EXP1:
    case DOUB_EXP2:
        CHECK_AND_CALL_FUNCTION(expr());  //FIX MY add -----
        CHECK_AND_CALL_FUNCTION(funcParamN());
        symtable = smSearchNode(root_symtable, clipboard[1].attribute);
        if (symtable != NULL) // FIXED исправить поиск, добавить поиск параметров
        {
            for (int i = 0; symtable->param[i].name != NULL; i++)
            {
                if (strcmp(symtable->param[i].name, clipboard[1].attribute) == 0)
                {
                    generate_move("LF@", clipboard[1].attribute, char_type(change_enum(expressionStack.head.type)), expressionStack.head.attribute);
                    return OK;
                }
            }
        }
        if (symtable != NULL)
        {
            generate_move(char_type(symtable->scope), clipboard[1].attribute, char_type(change_enum(expressionStack.head.type)), expressionStack.head.attribute);
        }
        else
            return SYNTAX_ERROR;
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
typeVar change_type(char *type)
{
    if (strcmp(type, "integer") == 0)
    {
        return sINT;
    }
    else if (strcmp(type, "string") == 0)
    {
        return sSTR;
    }
    else if (strcmp(type, "local") == 0)
    {
        return sLOCAL;
    }
    return NO;
}
/**
 *checking the data type of the variable 
 *and the input data type
 **/
typeVar check_type()
{
    if(token.type == INT && symtable->type[0] == sINT)
    {
        return sINT;
    }
    else if (token.type == STR && symtable->type[0] == sSTR)
    {
        return sSTR;
    }
    return SYNTAX_ERROR;
}

typeVar change_enum (TokenType token)
{
    switch (token)
    {
    case E_NONTERM_STR:
        return sSTR;
    case E_NONTERM_INT:
        return sINT;
    default:
        return SYNTAX_ERROR;
    }
}


char *char_type(typeVar type)
{
    if (type == sINT)
    {
        return "int@";
    }
    else if (type == sSTR)
    {
        return "string@";
    }
    else if (type == sLOCAL)
    {
        return "LF@";
    }
    printf("+++SYTAX ERROR+++");
}

