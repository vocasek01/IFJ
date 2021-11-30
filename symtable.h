#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"

#pragma once

typedef enum
{
    NO,
    sINT,
    sSTR,
    FLOAT,
    sLOCAL,
    sGLOBAL,
} typeVar;

typedef struct Parametrs
{
    char *name;
    typeVar type;
}parametr;

typedef struct BSTNode
{
    char *name;            //name of function or variable
    char * data;           //value of variable(12, 2.2, "Hello")
    typeVar type;          //type of variable(int, double, str)
    typeVar scope;
    bool isFunction;       //node is function or variable
    struct Parametrs param[32];
    struct BSTNode * LPtr; //left node
    struct BSTNode * RPtr; //right node
}
BSTNodePtr;

void smInit(BSTNodePtr **root);
/**
 * adde new func
* @param parametr_name if name is unknown use NULL
* @param parametr_type if name is unknown use NO
**/
int smInsertFunctin(BSTNodePtr **root, char *name, typeVar type, char *parametr_name, typeVar parametr_type, int param_num);

int smInsertVariable(BSTNodePtr **root, char *name, char *data, typeVar type, typeVar scope);

BSTNodePtr *smSearchNode (BSTNodePtr *root, char *name);

void smDispose (BSTNodePtr **root);

void smDeleteFunction (BSTNodePtr **root);