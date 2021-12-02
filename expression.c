#include "expression.h"

int counter_of_vars = 0;

int find_index(TokenType a) {

    switch(a) {

        case ADD:
            return 0;
        case SUB:
            return 1;
        case MUL:
            return 2;
        case DIV: 
            return 3;
        case LBR:
            return 4;
        case RBR:
            return 5;
        case GT:
            return 6;
        case LT:
            return 7;
        case GTE:
            return 8;
        case LTE:
            return 9;
        case EQ:
            return 10;
        case NOTEQ:
            return 11;
        case IDENTIFICATOR:
        case INT:
        case DOUBLE:
        case STR:
            return 12;
        case E_STOP:
            return 13;

        default:
            break;


    }

}

Rule find_rule(TokenType a, TokenType b) {

    int x = find_index(a);
    int y = find_index(b);
    return precedence[x][y];
}

int convert_to_nonterm(BSTNodePtr *root, Stack *tokenStack) {

    Token x = stackTop(tokenStack);
    switch (x.type)
    {
    case E_NONTERM_INT:
    case E_NONTERM_FLOAT:
    case E_NONTERM_BOOL:
    case E_NONTERM_STR:

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
        case EQ:
            convert_operation(root, tokenStack);
            return 0;
            break;
        
        default:
            break;
        }


        break;

    case IDENTIFICATOR:
        convert_id(root, tokenStack);
        return 0;
        break;
    case STR:
    case INT:
    case DOUBLE:
        convert_str(root, tokenStack);
        return 0;
        break;
    default:
        break;
    }


}

int convert_operation(BSTNodePtr *root, Stack *tokenStack) {

    Token right = stackTop(tokenStack);
    stackPop(tokenStack); //pop right
    Token operator = stackTop(tokenStack);
    stackPop(tokenStack); //pop operator
    Token left = stackTop(tokenStack);
    stackPop(tokenStack); //pop left
    stackPop(tokenStack); //pop shift
    Token result;

    char* var_name = malloc(sizeof(char) * 10);
    sprintf(var_name,"var%i",counter_of_vars++);
    result.attribute = var_name;
    generate_declaration("LF@",var_name);

    if (right.type != left.type) {
        fprintf(stderr, "%s and %s have different types\n",right.attribute,left.attribute);
    }

    int error_code;
    switch (right.type)
    {
        case E_NONTERM_STR:
            error_code = generate_arithmetic_operation_string(operator,var_name,left.attribute,right.attribute);
            break;
        case E_NONTERM_INT:
            error_code = generate_arithmetic_operation_int(operator,var_name,left.attribute,right.attribute);
            break;
        case E_NONTERM_BOOL:
            error_code = generate_arithmetic_operation_bool(operator,var_name,left.attribute,right.attribute);
            break;
        case E_NONTERM_FLOAT:
            error_code = generate_arithmetic_operation_float(operator,var_name,left.attribute,right.attribute);
            break;

        default:
            error_code = 1;
    }
    
    if (error_code) {
        return 2;
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
            break;
        case INT:
            prefix = "integer@";
            break;
        default:
            break;
    }
    char* var_name = malloc(sizeof(char) * 10);
    sprintf(var_name,"var%i",counter_of_vars++);

    generate_declaration("LF@",var_name);
    generate_move("LF@", var_name, prefix, x.attribute);

    switch (x.type)
    {
        case STR:
            x.type = E_NONTERM_STR;
            break;
        case DOUBLE:
            x.type = E_NONTERM_FLOAT;
            break;
        case INT:
            x.type = E_NONTERM_INT;
            break;
        default:
            break;
    }

    stackPush(tokenStack, x);

    return 0;
}




int convert_id(BSTNodePtr *root, Stack *tokenStack) {

    Token x = stackTop(tokenStack);
    BSTNodePtr *node = smSearchNode(root, x.attribute);
    stackPop(tokenStack); //pop id
    stackPop(tokenStack); //pop shift

    if (node == NULL) {
        fprintf(stderr, "uninitialized variable");
        return ERROR;
    }

    // x.type = node->type;
    switch (node->type)
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
        case E_SHIFT:
            break;
        
        default:
            found = 1;
            return tmp->head;
        }
        tmp = tmp->next;
    }

}
