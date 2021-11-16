#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

void smInit(BSTNodePtr **root) {
    *root = NULL;
}

int smInsertFunctin(BSTNodePtr **root, char *name) {
    BSTNodePtr *item = (BSTNodePtr *) malloc(sizeof(struct BSTNode));

    if (item == NULL) {
        return 99;
    }

    item->isFunction = true;
    item->name = name;
    item->data = NULL;
    item->LPtr = NULL;
    item->RPtr = *root;

    *root = item;

    return 0;
}

int smInsertVariable(BSTNodePtr **root, char *name, char *data, int type) {
    if (*root != NULL) {

        if (strcmp((*root)->name, name) > 0) {
            smInsertFunctin(&(*root)->RPtr, name);
        } else if (strcmp((*root)->name, name) < 0) {
            smInsertFunctin(&(*root)->LPtr, name);
        } else {
            (*root)->data = data;
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
                item = smSearchNode(root->RPtr, name);
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

int main() {
    BSTNodePtr **root;
    root = malloc(sizeof(BSTNodePtr));

    smInit(root);
    smInsertFunctin(root, "lolf");
    smInsertVariable(&((*root)->LPtr), "lolname1", "loldata", 1);
    smInsertVariable(&((*root)->LPtr), "lolname2", "loldata", 1);
    
    printf("%s %s\n", (*root)->LPtr->name, (*root)->LPtr->data);
    smDeleteFunction(root);

    smDispose(root);
    free(root);
    return 0;
}