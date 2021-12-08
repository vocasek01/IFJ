/*------------------------------------------
 Title:  expression.h
 Author: Koval Maksym
 Login:  xkoval20
 Date:   23 Nov 2021;
------------------------------------------*/

#include "precedence.h"
#include "codegen.h"

typedef enum{

    A, // convert left operator to type of right    example: 5 + 4.9 -> int2float(5) + 4.9
    B, // convert right operator to type of left    example: 2.1 + 4 -> 2.1 + int2float(4)
    C, // compatibility is okay    example : (1 + 2)
    D = ERR_COMPATIBILITY_IN_OPERATIONS, // error    example: 1 + "a"

} Compability;

int compability[6][6] = {
    //           INT   FLOAT STR   BOOL  NIL   ZERO
    /* INT   */ {C,    A,    D,    D,    C,    C},
    /* FLOAT */ {B,    C,    X,    D,    C,    C},
    /* STR   */ {D,    D,    C,    D,    C,    D},
    /* BOOL  */ {D,    D,    D,    C,    D,    D},
    /* NIL   */ {C,    C,    C,    D,    C,    C},
    /* ZERO  */ {C,    C,    D,    D,    C,    C}

};

int find_index(TokenType a);
int find_rule(TokenType a, TokenType b);
int types_compability(BSTNodePtr *root, Token a, Token b, Token *nameFunc, int counter_func);
int find_type(Token a);
int convert_id(BSTNodePtr *root, Stack *tokenStack, Token *nameFunc, int counter_func);
int convert_str(BSTNodePtr *root, Stack *tokenStack);
int convert_to_nonterm(BSTNodePtr *root, Stack *tokenStack,Token *nameFunc, int counter_func);
int convert_operation(BSTNodePtr *root, Stack *tokenStack, Token *nameFunc, int counter_func);
int convert_len(Stack *tokenStack);
int convert_nil(Stack *tokenStack);
int convert_parentheses(Stack *tokenStack);
Token find_term(Stack *tokenStack);
int isShifted(Token a);
int check_var(BSTNodePtr *root, char *name);
BSTNodePtr *checking_searching(BSTNodePtr *node, char *name);
