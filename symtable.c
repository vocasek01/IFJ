#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

void smInit(BSTNodePtr **root) {
    *root = NULL;
}

int smInsertFunctin(BSTNodePtr **root, char *name, typeVar type) {
    BSTNodePtr *item = (BSTNodePtr *) malloc(sizeof(struct BSTNode));

    if (item == NULL) {
        return 99;
    }

    item->isFunction = true;
    item->type = type;
    item->name = name;
    item->data = NULL;
    item->LPtr = NULL;
    item->RPtr = *root;

    *root = item;

    return 0;
}

int smInsertVariable(BSTNodePtr **root, char *name, char *data, typeVar type, typeVar scope) {
    if (*root != NULL) {

        if (strcmp((*root)->name, name) > 0) {
            smInsertVariable(&(*root)->RPtr, name, data, type, scope);
        } else if (strcmp((*root)->name, name) < 0) {
            smInsertVariable(&(*root)->LPtr, name, data, type, scope);
        } else {
            (*root)->data = data;
            (*root)->type = type;
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
        item->type = type;
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