#include <stdio.h>
#include <stdlib.h>
#include "scanner.c"
#include "scanner.h"
#pragma once

typedef struct bst_node
{
    Token token;
    int a;                // kľúč
    struct bst_node *op1;       // condition
    struct bst_node *op2;       // inside
    struct bst_node *op3;     // else
    struct bst_node *next;    // next
} bst_node_t;

void add_next(bst_node_t **tree);
void cre_node(bst_node_t **tree);
void bst_init(bst_node_t *tree);
void add_op2();