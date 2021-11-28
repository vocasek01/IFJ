#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"

#pragma once

typedef enum
{
    integer,
    string,
    FlOAT,
    NO,
} typeVar;

typedef struct BSTNode
{
    char *name;            //name of function or variable
    char * data;           //value of variable(12, 2.2, "Hello")
    typeVar type;           //type of variable(int, double, str)
    bool isFunction;       //node is function or variable
    struct BSTNode * LPtr; //left node
    struct BSTNode * RPtr; //right node
}
BSTNodePtr;

void smInit(BSTNodePtr **root);

int smInsertFunctin(BSTNodePtr **root, char *name, typeVar type);

int smInsertVariable(BSTNodePtr **root, char *name, char *data, typeVar type);

BSTNodePtr *smSearchNode (BSTNodePtr *root, char *name);

void smDispose (BSTNodePtr **root);

void smDeleteFunction (BSTNodePtr **root);