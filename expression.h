// #include "scanner.h"
#include "precedence.h"
#include "stack.h"
#include "symtable.h"
#include "codegen.c" // FIX MY


int find_index(TokenType a);
Rule find_rule(TokenType a, TokenType b);
int convert_id(BSTNodePtr *root, Stack *tokenStack);
int convert_str(BSTNodePtr *root, Stack *tokenStack);
int convert_to_nonterm(BSTNodePtr *root, Stack *tokenStack);
int convert_operation(BSTNodePtr *root, Stack *tokenStack);
Token find_term(Stack *tokenStack);