/*------------------------------------------
 Title:  parser.c
 Author: Matěj Krátký, Koval Maksym, Kirillov Artem
 Login:  xkratk17, xkoval20, xkiril01
 Date:   23 Nov 2021;
------------------------------------------*/

#include "parser.h"

Token token;
BSTNodePtr *symtable;
BSTNodePtr *root_symtable;
Token clipboard [16];
Token nameFunc [16];
Token callFunc;
int counter_func = -1;
int counter_if = -1;
int counter_while = -1;

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
    CHECK_AND_CALL_FUNCTION(firstBody());

    smDispose(&root_symtable);
    return OK;
}

int preamble()
{
    // rule <preamble> -> require "ifj21"

    checkAndLoadKeyword(KEYWORD, "require");

    if (token.type != IFJ21)
        return 7;

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
    // CHECK_AND_CALL_FUNCTION(chek_name(token.attribute));
    checkAndLoadToken(IDENTIFICATOR);
    checkAndLoadToken(LBR);
    CHECK_AND_CALL_FUNCTION(params());
    checkAndLoadToken(RBR);
    count_bracket = 2;
    CHECK_AND_CALL_FUNCTION(funcTypes());
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
        CHECK_AND_CALL_FUNCTION(stateList());
        symtable = root_symtable;
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
        return OK;


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

    counter_param++;
    smInsertFunctin(&symtable, nameFunc[counter_func].attribute, NO, token.attribute, NO, counter_param-1, 0);
    generate_func_param(token.attribute, counter_param);

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
            CHECK_AND_CALL_FUNCTION(state());
            CHECK_AND_CALL_FUNCTION(stateList());
            return OK;
            break;
        } // Rule: <state_list>->eps
        else if (strcmp(token.attribute, "end") == 0)
        {
            symtable = root_symtable;
            if (symtable->name != NULL)
            {
                if ((symtable->name[0] == 'i' && symtable->name[1] == 'f') || (symtable->name[0] == 'w' && symtable->name[1] == 'h' && symtable->name[2] == 'i' && symtable->name[3] == 'l'))
                    return OK;
            }
            generate_func_bottom(symtable->name);
            return OK;

            break;
        } // Rule: <state_list>->return <return>
        else if (strcmp(token.attribute, "return") == 0)
        {
            checkAndLoadKeyword(KEYWORD, "return");
            symtable = root_symtable;
            smAddRetVal(&symtable, token.attribute, counter_retVal++);
            CHECK_AND_CALL_FUNCTION(returnState());
            for (int i=0; symtable->retVal[i].name != NULL; i++)
            {
                char a[10];
                sprintf(a, "%%retval%d", i);
                generate_move("LF@", a, "LF@", symtable->retVal[i].name);
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
            CHECK_AND_CALL_FUNCTION(returnStateT36());
            for (int i = 0; symtable->retVal[i].name != NULL; i++)
            {
                char a[10];
                sprintf(a, "%%retval%d", i);
                generate_move("LF@", a, "LF@", symtable->retVal[i].name);
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
{
    switch (token.type)
    {
    case KEYWORD:
        if (strcmp(token.attribute, "integer") == 0)
        {
            decType();
            checkAndLoadKeyword(KEYWORD, "integer");
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "string") == 0)
        {
            decType();
            checkAndLoadKeyword(KEYWORD, "string");
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "nil") == 0)
        {
            decType();
            checkAndLoadKeyword(KEYWORD, "nil");
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "number") == 0)
        {
            decType();
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

int decType()
{
    if (symtable->isFunction != true)
    {
        symtable = smSearchNode(symtable, clipboard[1].attribute);
        smInsertVariable(&symtable, clipboard[1].attribute, NULL, change_type(token.attribute), change_type(clipboard[0].attribute));
    }
    else if (symtable->isFunction == true)
    {
        if (count_bracket == 2)
        {
            smInsertFunctin(&symtable, nameFunc[counter_func].attribute, change_type(token.attribute), symtable->param[0].name, symtable->param[0].type, -1, counter_type++);
        }
        else
            smInsertFunctin(&symtable, nameFunc[counter_func].attribute, NO, symtable->param[counter_param - 1].name, change_type(token.attribute), counter_param - 1, 0);
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
        clipboard[0] = token;
        checkAndLoadToken(IDENTIFICATOR);
        CHECK_AND_CALL_FUNCTION(check_dec(clipboard[0].attribute, 1));
        CHECK_AND_CALL_FUNCTION(afterID());
        expressionStack = stackReverse(&expressionStack);
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
            clipboard[1] = token;
            CHECK_AND_CALL_FUNCTION(check_dec(token.attribute, 0));
            smInsertVariable(&root_symtable, clipboard[1].attribute, NULL, NO, change_type(clipboard[0].attribute)); //ERROR&
            symtable = smSearchNode(root_symtable, clipboard[1].attribute);
            checkAndLoadToken(IDENTIFICATOR);
            checkAndLoadToken(COLON);
            CHECK_AND_CALL_FUNCTION(typesT20());
            CHECK_AND_CALL_FUNCTION(isAssign());
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
            checkAndLoadKeyword(KEYWORD, "end");
            return OK;
            break;
        }
        else if (strcmp(token.attribute, "while") == 0) // Rule: <state> ->  while <expression> do <state_list> end
        {
            symtable = root_symtable;
            counter_while++;
            char name_while[9];
            sprintf(name_while, "while%d", counter_while);
            smInsertFunctin(&symtable, name_while, NO, NULL, NO, 0, 0);
            root_symtable = symtable;
            checkInsertAndLoadToken(KEYWORD, "while");

            CHECK_AND_CALL_FUNCTION(expr());
            generate_while_head(stackTop(&expressionStack).attribute, counter_while);
            generate_while_label_cycle(counter_while);
            checkAndLoadKeyword(KEYWORD, "do");
            CHECK_AND_CALL_FUNCTION(stateList());

            generate_while_iterate(stackTop(&expressionStack).attribute, counter_while);
            generate_while_label_end(counter_while);
            smDeleteFunction(&symtable);
            root_symtable = symtable;
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
        expressionStack = stackReverse(&expressionStack);
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
        generate_move("LF@", symtable->name, char_type(symtable->type[0]), nil(symtable->type[0]));
        return OK;
        break;
    case KEYWORD:
        if (strcmp(token.attribute, "while") == 0 || strcmp(token.attribute, "if") == 0 || strcmp(token.attribute, "local") == 0 
        || strcmp(token.attribute, "return") == 0 || strcmp(token.attribute, "end") == 0 ) // Rule: <return>  ->  eps
        {
            generate_move("LF@", symtable->name, char_type(symtable->type[0]), nil(symtable->type[0]));
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
        if (token.type == IDENTIFICATOR)
        {
            CHECK_AND_CALL_FUNCTION(check_dec(token.attribute, 1));
        }

        smInsertVariable(&symtable, clipboard[1].attribute, token.attribute, symtable->type[0], change_type(clipboard[0].attribute));
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
    int brackets_number = 0;
    int finish = 0;
    Stack tokenStack;
    Token tmp, a, b, shift;
    shift.attribute = NULL;
    shift.type = E_SHIFT;

    stackInit(&tokenStack);
    tmp.type = E_STOP;
    tmp.attribute = NULL;

    stackPush(&tokenStack,tmp);

    while (true) {

        if (token.type == ERROR) {
            return LEX_ERROR;
        }
        a = find_term(&tokenStack);

        if (token.type == KEYWORD && strcmp(token.attribute, "nil") != 0 || token.type == COMMA || token.type == RBR && brackets_number == 0 || finish) { // FIXME
            b.type = E_STOP;
        }else {
            b = token;        
        }

        if (b.type == E_STOP && find_term(&tokenStack).type == E_STOP) {
            break;
        }

        int rule = find_rule(a.type, b.type);
        if (rule == ERROR) return ERROR;

        switch (rule)
        {
        
        case E:
            // pushes b token to stack
            stackPush(&tokenStack,b);
            
            if (b.type == LBR) brackets_number++;
            else if (b.type == RBR) brackets_number--;

            NEXT();
            break;

        case S:
            // pushes shift before or after token 
            tmp = stackTop(&tokenStack);

            if (isShifted(tmp)) {
                stackPop(&tokenStack);
            }

            stackPush(&tokenStack,shift);

            if (b.type == LBR) brackets_number++;
            else if (b.type == RBR) brackets_number--;

            if (isShifted(tmp)) {
                stackPush(&tokenStack,tmp);
            }

            stackPush(&tokenStack,b);
            NEXT();
            break;

        case L:
            // takes tokens from stack and convert to term (i+i) -> (E) -> E
            CHECK_AND_CALL_FUNCTION(convert_to_nonterm(root_symtable, &tokenStack, nameFunc, counter_func));
            break;

        case X:
            // unreal combination of expression tokens
            return ERROR;
        case F:
            // finish
            finish = 1;
            break;
        }

    }

    Token expression = stackTop(&tokenStack);
    stackPop(&tokenStack);
    stackPush(&expressionStack,expression);
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
    generate_declaration(char_type(symtable->scope), symtable->name);
    switch (token.type)
    {
    case IDENTIFICATOR: // Rule: <declr>     ->  id ( <func_param> )
        clipboard[2] = token;
        if (smSearchNode(root_symtable, token.attribute) != NULL && strcmp("readi", token.attribute) != 0)
        {
            symtable = smSearchNode(root_symtable, token.attribute);
            CHECK_AND_CALL_FUNCTION(check_dec(token.attribute, 1));
            if (symtable->isFunction == true)
            {
                callFunc = token;
                checkAndLoadToken(IDENTIFICATOR);
            }
        }

        if (token.type == LBR)
        {
            checkAndLoadToken(LBR);
            CHECK_AND_CALL_FUNCTION(funcParam());
            checkAndLoadToken(RBR);

            int countP = 0;
            char charParam[32];
            generate_frame();
            for (int i = 0; symtable->param[i].name != NULL; i++)
            {
                sprintf(charParam, "%%%d", i + 1);
                generate_declaration("TF@", charParam);
                generate_move("TF@", charParam, "LF@", expressionStack.head.attribute);
                stackPop(&expressionStack);
            }

            generate_function_call(callFunc.attribute);

            symtable = smSearchNode(root_symtable, callFunc.attribute);
            for (int i = 0; symtable->type[i] != NO; i++)
            {
                char charRetval[10];
                sprintf(charRetval, "%%retval%d", i);
                generate_move("LF@", clipboard[1].attribute, "TF@", charRetval);
            }
            symtable = root_symtable;
            callFunc.attribute = NULL;
        }
        else // Rule: <declr>     ->  <expression>
        {
            if (strcmp("readi", token.attribute) == 0)
            {
                generate_readi("LF@", clipboard[1].attribute, "int@0");
                checkAndLoadToken(IDENTIFICATOR);
                checkAndLoadToken(LBR);
                checkAndLoadToken(RBR);
                return OK;
            }

            CHECK_AND_CALL_FUNCTION(expr());
            generate_move("LF@", symtable->name, "LF@", expressionStack.head.attribute);
            stackPop(&expressionStack);
        }
        return OK;
        break;
    case KEYWORD:
        if (strcmp(token.attribute, "nil") != 0)
            return SYNTAX_ERROR;

        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprNT40());

        CHECK_AND_CALL_FUNCTION(check_type());
        if (expressionStack.head.type == E_NONTERM_NIL)
        {
            generate_move(char_type(symtable->scope), symtable->name, "LF@", expressionStack.head.attribute);
        }
        // else{
        //     generate_move(char_type(symtable->scope), symtable->name, "LF@", expressionStack.head.attribute);
        // }
        stackPop(&expressionStack);
        return OK;
        break;
    case INT:
    case DOUBLE:
    case STR:
    case KONC:
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(exprNT40());

        CHECK_AND_CALL_FUNCTION(check_type());
        generate_move(char_type(symtable->scope), symtable->name, "LF@", expressionStack.head.attribute);
        stackPop(&expressionStack);
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
        clipboard[1] = token;
        if (smSearcParamFunc(root_symtable, token.attribute) == NULL )
        {
            symtable = smSearchNode(root_symtable, token.attribute);    
            CHECK_AND_CALL_FUNCTION(check_dec(token.attribute,1));
            if (smSearchNode(root_symtable, token.attribute)->isFunction == true)
            {
                callFunc = token;
                checkAndLoadToken(IDENTIFICATOR);
            }
        }

        if (token.type == LBR)
        {
            checkAndLoadToken(LBR);
            CHECK_AND_CALL_FUNCTION(funcParam());
            checkAndLoadToken(RBR);

            int countP = 0;
            char charParam[32];
            generate_frame();
            for(int i=0; symtable->param[i].name != NULL; i++)
            {
                sprintf(charParam, "%%%d", i+1);
                generate_declaration("TF@", charParam);
                generate_move("TF@", charParam, "LF@", expressionStack.head.attribute);
                stackPop(&expressionStack);
            }

            generate_function_call(callFunc.attribute);

            symtable = smSearchNode(root_symtable, callFunc.attribute);
            for(int i=0; symtable->type[i] != NO; i++)
            {
                char charRetval[10];
                sprintf(charRetval, "%%retval%d", i);
                generate_move("LF@", clipboard[0].attribute,"TF@", charRetval);
            }
            symtable = root_symtable;
            callFunc.attribute = NULL;
            

            
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
                    generate_move(char_type(symtable->scope), symtable->name, "LF@", expressionStack.head.attribute);
                    stackPop(&expressionStack);
                    return OK;
                }
                else
                    return SYNTAX_ERROR;
            }
            generate_move("LF@", smSearcParamFunc(root_symtable, clipboard[0].attribute)->name, "LF@", expressionStack.head.attribute);
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

        if (symtable != NULL)
        {
            for (int i=0; symtable->param[i].name != NULL; i++)
            {
                if (strcmp(symtable->param[i].name, clipboard[0].attribute) == 0)
                {
                    generate_move("LF@", clipboard[0].attribute, "LF@", expressionStack.head.attribute);
                    stackPop(&expressionStack);
                    return OK;
                }
            }
        }
        if (symtable != NULL)
        {
            generate_move(char_type(symtable->scope), clipboard[0].attribute, "LF@", expressionStack.head.attribute);
            stackPop(&expressionStack);
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
        CHECK_AND_CALL_FUNCTION(exprNT56());
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
        CHECK_AND_CALL_FUNCTION(expr());
        CHECK_AND_CALL_FUNCTION(funcParamN());
        if(callFunc.attribute == NULL)
        {
            if (strcmp(clipboard[0].attribute, "write") == 0)
            return OK;
            
            symtable = smSearchNode(root_symtable, clipboard[1].attribute);
            if (symtable != NULL)
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
    
        }
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
    else if (strcmp(type, "number") == 0)
    {
        return FLOAT;
    }
    else if (strcmp(type, "nil") == 0)
    {
        return sNULL;
    }
    else if (strcmp(type, "local") == 0)
    {
        return sLOCAL;
    }
    return NO;
}
/**
 *checking for error 4
 * 4 - sémantická chyba v příkazu přiřazení (typová nekompatibilita).
 **/
int check_type()
{
    if ((expressionStack.head.type == E_NONTERM_INT || expressionStack.head.type == E_NONTERM_NIL) && symtable->type[0] == sINT)
    {
        return OK;
    }
    else if ((expressionStack.head.type == E_NONTERM_STR || expressionStack.head.type == E_NONTERM_NIL) && symtable->type[0] == sSTR)
    {
        return OK;
    }
    else if ((expressionStack.head.type == E_NONTERM_FLOAT || expressionStack.head.type == E_NONTERM_INT || expressionStack.head.type == E_NONTERM_NIL) && symtable->type[0] == FLOAT)
    {
        return OK;
    }
    else if (expressionStack.head.type == E_NONTERM_NIL && symtable->type[0] == sNULL)
    {
        return OK;
    }
    else if (expressionStack.head.attribute == NULL)
    {
        return OK;
    }
    return 4; //• 4 - sémantická chyba v příkazu přiřazení (typová nekompatibilita).
}

/**
 *checking for error 3
 *3 - pokus o redefinici proměnné -
 *  {"do", "else", "end", "function",
 *    "global", "if", "integer", "local",
 *   "nil", "number", "require", "return",
 *  "string", "then", "while"};.
 *
 **/
int chek_name(char *name)
{
    char *dec_name[] = 
    {"do", "else", "end", "function",
    "global", "if", "integer", "local", 
    "nil", "number", "require", "return", 
    "string", "then", "while"};

    for (int i = 0; i < 15; i++)
    {
        if (strcmp(name, dec_name[i]) == 0)
            return 2;
    }
    return OK;
}

int build_func(char *name)
{
    char *func_name[] =
    {"write", "readi"};
    for (int i = 0; i < 2; i++)
    {
        if (strcmp(name, func_name[i]) == 0)
            return OK;
    }
    return 699;
}
/**
 *checking for error
 *3 - sémantická chyba v programu – nedefinovaná funkce/proměnná,
 *pokus o redefinici proměnné, atp.
 *@param a can have 0 or 1
 *  @param 0 checking whether the declared variable was, if it does not exist, returns OK
 *  @param 1 checking for the presence of a variable, if it exists, returns OK
 **/
int check_dec(char *name, int a)
{   
    if (!build_func(name))
        return OK;

    if (a == 0)
    {
        CHECK_AND_CALL_FUNCTION(chek_name(name));
        if (smSearchNode(root_symtable, name) != NULL)
        {
            symtable = smSearchNode(root_symtable, nameFunc[counter_func].attribute);
            if (smChekVar(symtable, name) != NULL)
                return ERR_UNDEFINED_VARIABLE;

            symtable = root_symtable;

            for (int i = 0; nameFunc[i].attribute != NULL; i++)
            {
                if (strcmp(name, nameFunc[i].attribute) == 0)
                    return ERR_UNDEFINED_VARIABLE;
            }
        }
        else if (smSearchParamNode(root_symtable, name) != NULL)
        {
            if (smSearcParamFunc(smSearchNode(root_symtable, nameFunc[counter_func].attribute), name) != NULL)
            {
                return ERR_UNDEFINED_VARIABLE;
            }
        }
        return OK;
    }
    if (a == 1)
    {
        if (smSearchNode(root_symtable, nameFunc[counter_func].attribute) != NULL)
        {
            if (smChekVar(smSearchNode(root_symtable, nameFunc[counter_func].attribute), name) != NULL)
            {
                return OK; //• 3 - sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici proměnné, atp.
            }

            if (smSearchNode(root_symtable, name) != NULL && smSearchNode(root_symtable, name)->isFunction == true)
            {
                return OK;
            }

            if (smSearcParamFunc(smSearchNode(root_symtable, nameFunc[counter_func].attribute), name) != NULL)
            {
                return OK;
            }
        }
        return ERR_UNDEFINED_VARIABLE;
    }
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

char* nil(typeVar type)
{
    switch (type)
    {
    case sINT:
        return "0";

    case sSTR:
        return "nil";

    case FLOAT:
        return "0x0p+0";

    case sNULL:
        return "nil";

    default:
        break;
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
    else if(type == FLOAT)
    {
        return "float@";
    }
    else if (type == sLOCAL || type == NO)
    {
        return "LF@";
    }
    else if (type == sNULL)
    {
        return "nil@";
    }
    printf("+++SYTAX ERROR+++");
}

