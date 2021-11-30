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
        case STOP:
            return 13;

        default:
            break;


    }

}

Rule find_rule(TokenType a, TokenType b) {

    int a = find_index(a);
    int b = find_index(b);
    return precedence[a][b];
}





