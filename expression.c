#include "expression.h"

int counter_of_vars = 0;
BSTNodePtr item;

int find_index(TokenType a) {

    switch(a) {

        case LEN:
            return 0;
        case ADD:
            return 1;
        case SUB:
            return 2;
        case MUL:
            return 3;
        case DIV: 
            return 4;
        case INT_DIV:
            return 5;
        case KONC:
            return 6;
        case LBR:
            return 7;
        case RBR:
            return 8;
        case GT:
            return 9;
        case LT:
            return 10;
        case GTE:
            return 11;
        case LTE:
            return 12;
        case EQ:
            return 13;
        case NOTEQ:
            return 14;
        case KEYWORD: 
        case IDENTIFICATOR:
        case INT:
        case DOUBLE:
        case STR:
            return 15;
        case E_STOP:
            return 16;

        default:
            return ERROR;          


    }

}

int find_rule(TokenType a, TokenType b) {

    int x = find_index(a);
    int y = find_index(b);
    if (x == ERROR || y == ERROR) return 0;
    return precedence[x][y];
}

int convert_to_nonterm(BSTNodePtr *root, Stack *tokenStack,Token *nameFunc, int counter_func) {

    Token x = stackTop(tokenStack);
    switch (x.type)
    {
    case E_NONTERM_INT:
    case E_NONTERM_FLOAT:
    case E_NONTERM_BOOL:
    case E_NONTERM_STR:
    case E_NONTERM_NIL:

        x = stackTop(tokenStack->next);
        switch (x.type)
        {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case INT_DIV:
        case GT:
        case LT:
        case NOTEQ:
        case GTE:
        case LTE:
        case KONC:
        case EQ:
            CHECK_AND_CALL_FUNCTION(convert_operation(root, tokenStack, nameFunc, counter_func));
            return OK;
        case LEN:
            CHECK_AND_CALL_FUNCTION(convert_len(tokenStack));
            return OK;
        default:
            return ERR_COMPATIBILITY_IN_OPERATIONS;
        }
        break;

    case IDENTIFICATOR:
        CHECK_AND_CALL_FUNCTION(convert_id(root, tokenStack, nameFunc, counter_func));
        return 0;
    case STR:
    case INT:
    case DOUBLE:
        CHECK_AND_CALL_FUNCTION(convert_str(root, tokenStack));
        return OK;
    case RBR:
        CHECK_AND_CALL_FUNCTION(convert_parentheses(tokenStack));
        return OK;
    case KEYWORD:
        CHECK_AND_CALL_FUNCTION(convert_nil(tokenStack));
        return OK;
    default:
        break;
    }

    return 0;
}

int convert_len(Stack *tokenStack) {

    Token x = stackTop(tokenStack);
    stackPop(tokenStack); //pop operand
    stackPop(tokenStack); //pop len
    stackPop(tokenStack); //pop shift

    if (x.type != E_NONTERM_STR) return ERR_COMPATIBILITY_IN_OPERATIONS;

    char* var_name = malloc(sizeof(char) * 10);
    if (var_name == NULL) return ERR_ALLOCATION_ERROR_OR_ETC;

    sprintf(var_name,"var%i",counter_of_vars++);
    generate_declaration("LF@", var_name);

    generate_len(var_name,x.attribute);
    free(x.attribute);
    x.attribute = var_name;
    x.type = E_NONTERM_INT;
    stackPush(tokenStack,x);

    return OK;
} 

int convert_nil(Stack *tokenStack) {

    Token x = stackTop(tokenStack);
    stackPop(tokenStack); //pop nil
    stackPop(tokenStack); //pop shift
    
    x.type = E_NONTERM_NIL; 

    stackPush(tokenStack, x);
    return 0;
}

int convert_parentheses(Stack *tokenStack) {

    Token right = stackTop(tokenStack);
    stackPop(tokenStack); //pop right
    Token term = stackTop(tokenStack);
    stackPop(tokenStack); //pop operator
    Token left = stackTop(tokenStack);
    stackPop(tokenStack); //pop left
    stackPop(tokenStack); //pop shift

    stackPush(tokenStack,term);

    return 0;
}


int types_compability(BSTNodePtr *root, Token a, Token b, Token *nameFunc, int counter_func) {

    BSTNodePtr *link_node;
    BSTNodePtr *node;
    switch(compability[a.type][b.type]) {
        case A:
            generate_int2float(a.attribute);

            // find "a.attribute" in symtable and change type to sFLOAT
            // BSTNodePtr *node1 = smSearchNode(root_symtable, a.attribute);
            // node1->type[0] = FLOAT;
            link_node = smSearchNode(root, nameFunc[counter_func].attribute);
            CHECK_AND_CALL_FUNCTION(check_var(link_node, a.attribute));
            node = checking_searching(link_node, a.attribute);
            node->type[0] = FLOAT;
            // FORARTEM

            a.type = E_NONTERM_FLOAT;
            return OK;
        case B:
            // find "b.attribute" in symtable and change type to sFLOAT
            // BSTNodePtr *node2 = smSearchNode(root_symtable, b.attribute);
            // node2->type[0] = FLOAT;
            // FORARTEM

            link_node = smSearchNode(root, nameFunc[counter_func].attribute);
            CHECK_AND_CALL_FUNCTION(check_var(link_node, b.attribute));
            node = checking_searching(link_node, b.attribute);
            node->type[0] = FLOAT;

            generate_int2float(b.attribute);

            b.type = E_NONTERM_FLOAT;   
            return OK;
        case C:
            return OK;
        case D:
            return ERR_COMPATIBILITY_IN_OPERATIONS;
    }
}

int convert_operation(BSTNodePtr *root, Stack *tokenStack, Token *nameFunc, int counter_func) {

    int error_code = 0;
    Token right = stackTop(tokenStack);
    stackPop(tokenStack); //pop right
    Token operator = stackTop(tokenStack);
    stackPop(tokenStack); //pop operator
    Token left = stackTop(tokenStack);
    stackPop(tokenStack); //pop left
    stackPop(tokenStack); //pop shift
    Token result;

    char* var_name = malloc(sizeof(char) * 10);
    if (var_name == NULL) return ERR_ALLOCATION_ERROR_OR_ETC;

    sprintf(var_name,"var%i",counter_of_vars++);
    result.attribute = var_name;
    generate_declaration("LF@", var_name);

    CHECK_AND_CALL_FUNCTION(types_compability(root, right, left, nameFunc, counter_func));

    switch (right.type)
    {
        case E_NONTERM_STR:
            CHECK_AND_CALL_FUNCTION(generate_arithmetic_operation_string(operator,var_name,left.attribute,right.attribute));
            break;
        case E_NONTERM_INT:
            CHECK_AND_CALL_FUNCTION(generate_arithmetic_operation_int(operator,var_name,left.attribute,right.attribute));
            break;
        case E_NONTERM_BOOL:
            CHECK_AND_CALL_FUNCTION(generate_arithmetic_operation_bool(operator,var_name,left.attribute,right.attribute));
            break;
        case E_NONTERM_FLOAT:
            CHECK_AND_CALL_FUNCTION(generate_arithmetic_operation_float(operator,var_name,left.attribute,right.attribute));
            break;
         case E_NONTERM_NIL:
            CHECK_AND_CALL_FUNCTION(generate_arithmetic_operation_nil(operator,var_name,left.attribute,right.attribute));
            break;

        default:
            error_code = ERR_COMPATIBILITY_IN_OPERATIONS;
    }
    
    if (error_code) {
        return error_code;
    }

    switch (operator.type)
    {
        case GT:
        case LT:
        case NOTEQ:
        case GTE:
        case LTE:
        case EQ:
            result.type = E_NONTERM_BOOL;
            break;
        
        default:
            result.type = left.type;
            break;
    }

    stackPush(tokenStack,result);
    
    return 0;

}

int convert_str(BSTNodePtr *root, Stack *tokenStack) {

    int value = 0;
    Token x = stackTop(tokenStack);
    stackPop(tokenStack); //pop id
    stackPop(tokenStack); //pop shift

    char* prefix; // len = 7
        switch (x.type)
    {
        case STR:
            prefix = "string@";
            break;
        case DOUBLE:
            prefix = "float@";
            // FIXME float can be 0
            double attribute_in_double = atof(x.attribute);
            char* tmp = malloc(sizeof(char)*100);
            if (tmp == NULL) return ERR_ALLOCATION_ERROR_OR_ETC;
            free(x.attribute);
            sprintf(tmp,"%a",attribute_in_double);   
            x.attribute = tmp;         
            break;

        case INT:
            prefix = "int@";
            // if attribute will be zero, value will be non zero
            value = !atoi(x.attribute);

            break;
        default:
            break;
    }
    char* var_name = malloc(sizeof(char) * 10);
    if (var_name == NULL) return ERR_ALLOCATION_ERROR_OR_ETC;

    sprintf(var_name,"var%i",counter_of_vars++);

    generate_declaration("LF@",var_name);
    generate_move("LF@", var_name, prefix, x.attribute);

    free(x.attribute);
    x.attribute = var_name;

    switch (x.type)
    {
        case STR:
            x.type = E_NONTERM_STR;
            break;
        case DOUBLE:
            x.type = E_NONTERM_FLOAT;
            break;
        case INT:
            // FIXME zero division 
            // if (value) {
                // x.type = E_NONTERM_ZERO_INT;
            // } else {
                x.type = E_NONTERM_INT;
            // }
            break;
        default:
            break;
    }

    stackPush(tokenStack, x);

    return 0;
}

int convert_id(BSTNodePtr *root, Stack *tokenStack, Token *nameFunc, int counter_func)
{

    Token x = stackTop(tokenStack);
    BSTNodePtr *link_node = smSearchNode(root, nameFunc[counter_func].attribute);
    CHECK_AND_CALL_FUNCTION(check_var(link_node, x.attribute));
    BSTNodePtr *node = checking_searching(link_node, x.attribute);
    // parametr *param; 

    // CHECK_AND_CALL_FUNCTION(check_dec(smSearchNode(root, nameFunc[counter_func].attribute), 1));
    // if (node == NULL)
    // parametr *param = smSearcParamFunc(smSearchNode(root, nameFunc[counter_func].attribute), x.attribute);

        // FORARTEM
        // check_dec(x.attribute,1);

    stackPop(tokenStack); // pop id
    stackPop(tokenStack); //pop shift

    if (node == NULL) {
        fprintf(stderr, "uninitialized variable!\n");
        return ERROR;
    }

    // x.type = node->type;
    switch (node->type[0])
    {
        case sSTR:
            x.type = E_NONTERM_STR;
            break;
        case sINT:
            x.type = E_NONTERM_INT;
            break;
        case FLOAT:
            x.type = E_NONTERM_FLOAT;
            break;
        default:
            break;
    }

    stackPush(tokenStack, x);
    return 0;
}

Token find_term(Stack *tokenStack) {

    Token result;
    int found = 0;
    Stack *tmp = tokenStack;

    while(!found) {
        
        switch (tmp->head.type)
        
        {
        case E_NONTERM_RULE:
        case E_NONTERM_ID:
        case E_NONTERM_INT:
        case E_NONTERM_STR:
        case E_NONTERM_FLOAT:
        case E_NONTERM_BOOL:
        case E_NONTERM_NIL:
        case E_SHIFT:
            break;
        
        default:
            found = 1;
            return tmp->head;
        }
        tmp = tmp->next;
    }

}

int isShifted(Token a) {
    switch (a.type)
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
    case LBR:
    case RBR:
        return 0;
    default:
        return 1;
    }
}

BSTNodePtr *checking_searching(BSTNodePtr *node, char *name)
{   
    // CHECK_AND_CALL_FUNCTION(check_var(node, name));
    if (smSearcParamFunc(node, name) != NULL)
    {
        
        item.name = smSearcParamFunc(node, name)->name;
        item.type[0] = smSearcParamFunc(node, name)->type;
        item.isFunction = false;
        return &item;
    }
    node = smSearchNode(node, name);

    return node;
}

int check_var(BSTNodePtr *root, char *name)
{
    if (root != NULL)
    {
        if (smChekVar(root, name) != NULL)
        {
            return OK; //• 3 - sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici proměnné, atp.
        }

        if (smSearchNode(root, name) != NULL && smSearchNode(root, name)->isFunction == true)
        {
            return OK;
        }

        // if (smSearc)

        if (smSearcParamFunc(root, name) != NULL)
        {
            return OK;
        }
    }
    return 3;
}