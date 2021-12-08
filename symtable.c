/*------------------------------------------
 Title:  symtable.c
 Author: Golikov Ivan
 Login:  xgolik00 
 Date:   15 Oct 2021;
------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

void smInit(BSTNodePtr **root) {
    *root = NULL;
}

int smInsertFunctin(BSTNodePtr **root, char *name, typeVar type, char *parametr_name, typeVar parametr_type, int param_num, int type_num)
{
    if (*root != NULL)
    {
        if (strcmp((*root)->name, name) == 0)
        {
            if (param_num == -1)
            {
                (*root)->type[type_num] = type;
            }
            else if (param_num == -2)
            {
                
            }
            else{
                (*root)->param[param_num].name = parametr_name;
                (*root)->param[param_num].type = parametr_type;
                (*root)->type[type_num] = type;
            }
            return 0;
        }
    }
    BSTNodePtr *item = (BSTNodePtr *)malloc(sizeof(struct BSTNode));

    if (item == NULL) {
        return 99;
    }

    item->isFunction = true;
    item->param[param_num].name = parametr_name;
    item->param[param_num].type = parametr_type;    

    item->type[0] = type;
    item->name = name;
    item->data = NULL;
    item->LPtr = NULL;
    item->RPtr = *root;

    *root = item;

    return 0;
}

int smAddRetVal(BSTNodePtr **root, char *retVal, int num_ret)
{
    if (*root != NULL)
    {
        if ((*root)->isFunction == true)
        {
            (*root)->retVal[num_ret].name = retVal;
            return 0;
        }
        return 1;
    }
    return 1;
}

int smInsertVariable(BSTNodePtr **root, char *name, char *data, typeVar type, typeVar scope) {
    if (*root != NULL) {
        if ((*root)->isFunction == true)
        {
            smInsertVariable(&(*root)->LPtr, name, data, type, scope);
        }
        if (strcmp((*root)->name, name) > 0)
        {
            smInsertVariable(&(*root)->LPtr, name, data, type, scope);
        }
        else if (strcmp((*root)->name, name) < 0)
        {
            smInsertVariable(&(*root)->RPtr, name, data, type, scope);
        }
        else
        {
            (*root)->data = data;
            (*root)->type[0] = type;
            (*root)->scope = scope;
        }   

    } else {

        BSTNodePtr *item;
        item = (BSTNodePtr *) malloc(sizeof(struct BSTNode));

        if (item == NULL) {
            return 99;
        }
        
        item->data = data;
        item->name = name;
        item->type[0] = type;
        item->scope = scope;
        item->isFunction = false;
        item->LPtr = NULL;
        item->RPtr = NULL;

        *root = item;
    }

    return 0;
}

void smDispose (BSTNodePtr **root) {
    if (*root != NULL) {
        smDispose(&(*root)->LPtr);
        smDispose(&(*root)->RPtr);
        
        free(*root);
        root = NULL;
    }

    return;
}

BSTNodePtr *smSearchNode (BSTNodePtr *root, char *name) {
    BSTNodePtr *item = NULL;

    if (root != NULL)
    {
        if (strcmp(root->name, name) == 0)
        {
            return root;
        }
        if (root->isFunction == true)
        {
            item = smSearchNode(root->LPtr, name);
            if (item == NULL)
            {
                item = smSearchNode(root->RPtr, name);
            }

            return item;
        }
        else
        {
            if (strcmp(root->name, name) > 0)
            {
                item = smSearchNode(root->LPtr, name);
            }
            else if (strcmp(root->name, name) < 0)
            {
                item = smSearchNode(root->RPtr, name);
            }
            else
            {
                item = root;
            }
            return item;
        }
    }
    else
    {
        return NULL;
    }
}
/**
 * searches for a node that has a parameter
 *
 **/
BSTNodePtr *smSearchParamNode(BSTNodePtr *root, char *name)
{
    BSTNodePtr *item = NULL;

    if (root != NULL)
    {
        if (root->isFunction == true)
        {
            for (int i = 0; root->param[i].name != NULL; i++)
            {
                if (strcmp(root->param[i].name, name) == 0)
                {
                    return root;
                }
            }
        }
        else
        {
            item = smSearchParamNode(root->RPtr, name);
            return item;
        }
        return NULL;
    }
    else
    {
        return NULL;
    }
    return NULL;
}

parametr *smSearcParamFunc(BSTNodePtr *root, char *name)
{
    BSTNodePtr *item = NULL;
    item = root;
    if (item == NULL)
        return NULL;

    for (int i = 0; item->param[i].name != NULL; i++)
    {
        if (strcmp(item->param[i].name, name) == 0)
        {
            return &(item->param[i]);
        }
    }
    return NULL;
}

BSTNodePtr *smChekVar(BSTNodePtr *root, char *name)
{
    BSTNodePtr *item = NULL;

    if (root != NULL)
    {
            if (strcmp(root->name, name) != 0)
            {
                if (root->isFunction == true)
                {
                   item = smChekVar(root->LPtr, name);
                }
                else
                {
                    item = smSearchNode(root, name);
                }
            }
            else
            {
                item = root;
            }
            return item;
    }
    else
    {
        return NULL;
    }
    return NULL;
}

void smDeleteFunction (BSTNodePtr **root) {
    if (*root != NULL) {
        smDispose(&(*root)->LPtr);
        
        BSTNodePtr *delete = *root;

        *root = (*root)->RPtr;
        
        free(delete);
        delete = NULL;
    }

    return;
}