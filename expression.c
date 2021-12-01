#include "expression.h"


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
    case E_NONTERM:
        
        break;

    case IDENTIFICATOR:
        convert_id(root, tokenStack);
        return 0;
        break;

    default:
        break;
    }


}

int convert_id(BSTNodePtr *root, Stack *tokenStack) {

    Token x = stackTop(tokenStack);
    stackPop(tokenStack); //pop id
    stackPop(tokenStack); //pop shift

    BSTNodePtr *node = smSearchNode(root, x.attribute);

    if (node == NULL) {
        fprintf(stderr, "uninitialized variable");
        return ERROR;
    }

    // x.type = node->type;
    x.type = E_NONTERM;
    stackPush(tokenStack, x);
    return 0;
}


