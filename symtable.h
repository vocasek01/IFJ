#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#pragma once

typedef struct BSTNode {
    char *name;               //name of function or variable
    char *data;               //value of variable(12, 2.2, "Hello")
    int type;                 //type of variable(int, double, str)
    bool isFunction;          //node is function or variable
    struct BSTNode *LPtr;     //left node
    struct BSTNode *RPtr;     //right node
} BSTNodePtr;

void smInit(BSTNodePtr **root);

int smInsertFunctin(BSTNodePtr **root, char *name);

int smInsertVariable(BSTNodePtr **root, char *name, char *data, int type);

void smDispose (BSTNodePtr **root);

void smDeleteFunction (BSTNodePtr **root);