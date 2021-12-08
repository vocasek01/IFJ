// #include "scanner.h"
#include "precedence.h"
#include "stack.h"
#include "symtable.h"
#include "codegen.c" // FIX MY

// typedef enum{



// } RuleType;

typedef enum{

    A, // convert left operator to type of right    example: 5 + 4.9 -> int2float(5) + 4.9
    B, // convert right operator to type of left    example: 2.1 + 4 -> 2.1 + int2float(4)
    C, // compatibility is okay    example : (1 + 2)
    D = ERR_COMPATIBILITY_IN_OPERATIONS, // error    example: 1 + "a"

} Compability;

int compability[5][5] = {
    //           INT   FLOAT STR   BOOL  NIL
    /* INT   */ {C,    A,    D,    D,    C},
    /* FLOAT */ {B,    C,    X,    D,    C},
    /* STR   */ {D,    D,    C,    D,    C},
    /* BOOL  */ {D,    D,    D,    C,    D},
    /* NIL   */ {C,    C,    C,    D,    C}
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