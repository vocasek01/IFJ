#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

typedef struct bst_node
{
    Token token;                // kľúč
    struct bst_node *op1;       // condition
    struct bst_node *op2;       // inside
    struct bst_node *op3;     // else
    struct bst_node *next;    // next
} bst_node_t;