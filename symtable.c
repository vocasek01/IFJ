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
            // (*root)->isFunction = true;
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

        if (strcmp((*root)->name, name) > 0) {
            smInsertVariable(&(*root)->RPtr, name, data, type, scope);
        } else if (strcmp((*root)->name, name) < 0) {
            smInsertVariable(&(*root)->LPtr, name, data, type, scope);
        } else {
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

    if (root != NULL) {
        if (strcmp(root->name, name) == 0)
        {
            return root;
        }
        if (root->isFunction == true)
        {
            for (int i=0;root->param[i].name != NULL; i++)
            {
                if (strcmp(root->param[i].name, name) == 0)
                {
                    return root;
                }
            }
        }
        if (root->isFunction == true) {
            item = smSearchNode(root->LPtr, name);
            if (item == NULL) {
                item = smSearchNode(root->RPtr, name);
            }

            return item;
        } else {
            if (strcmp(root->name, name) > 0) {
                item = smSearchNode(root->RPtr, name);
            } else if (strcmp(root->name, name) < 0) {
                item = smSearchNode(root->LPtr, name);
            } else {
                item = root;
            }
            return item;
        }
    } else {
        return NULL;
    }
    
}

parametr smSearcParamFunc(BSTNodePtr *root, char *name)
{
    BSTNodePtr *item = NULL;
    item = smSearchNode(root, name);
    if (item == NULL)
        return item->param[32];

    for (int i = 0; item->param[i].name != NULL; i++)
    {
        if (strcmp(item->param[i].name, name) == 0)
        {
            return item->param[i];
        }
    }
    return item->param[32];
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

//after delete!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// int main() {
//     BSTNodePtr **root;
//     root = malloc(sizeof(BSTNodePtr));
//     BSTNodePtr *item;

//     smInit(root);
//     smInsertFunctin(root, "lolf1");
//     smInsertVariable(&((*root)->LPtr), "lolname1", "loldata1", 1);
//     smInsertVariable(&((*root)->LPtr), "lolname2", "loldata2", 1);

//     smInsertFunctin(root, "lolf2");
//     smInsertVariable(&((*root)->LPtr), "lolname3", "loldata3", 1);
//     smInsertVariable(&((*root)->LPtr), "lolname1", "loldata1", 1);

//     item = smSearchNode(*root, "lolname1");
//     printf("%s\n", item->name);

//     item = smSearchNode(*root, "lolname2");
//     printf("%s\n", item->name);

//     smDeleteFunction(root);

//     smDispose(root);
//     free(root);
//     return 0;
// }