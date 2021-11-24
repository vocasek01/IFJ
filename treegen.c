#include <stdio.h>
#include <stdlib.h>
#include "treegen.h"
#include "scanner.h"

// bst_node_t *tree1;

void create(bst_node_t **tree)
{
    Token token;
    // cre_node(&(*tree));
    do{
        token = getToken();
        // printf("type: %d atribut: %s \n----------------------------------------------------------------------\n", token.type, token.attribute);
        // if (token.attribute == "function")
        if (strcmp(token.attribute, "function") == 0)
        {
                cre_node(&(*tree));
                cre_node(&(*tree)->next);
                (*tree)->a = 1;
                (*tree)->next->a = 2;
                // printf("LOLOLOLOL\n");
        }
        if 
    } while (token.type != ENDOFFILE);
}

void cre_node(bst_node_t **tree)
{
    bst_node_t *item = NULL;
    item = (bst_node_t *)malloc(sizeof(struct bst_node));
    *tree = item;
}

void add_next(bst_node_t **tree)
{
    bst_node_t *item = NULL;
    item = (bst_node_t *)malloc(sizeof(struct bst_node));
    // tree = (bst_node_t *)malloc(sizeof(struct bst_node));
    item->a = 1;
    printf("LOL: %i", item->a);
    *tree = item;   
}
void add_op2()
{

}

void bst_init(bst_node_t *tree)
{
    tree = NULL;
}
int main(int argc, char *argv[])
{
    // int i = 0;
    argc = 2;
    argv[1] = "test_pr2.txt";
    scannerInit(argv[1]);
    bst_node_t **tree = NULL;
    tree = (bst_node_t **)malloc(sizeof(struct bst_node));
    create(tree);
    // add_next(tree);

    // // cre_node(&(*tree)->next);
    // (*tree)->a = 1;
    // // (*tree)->next->a = 2;
    // printf("LOL: %i", (*tree)->a);
    // add_next(&(*tree)->next);
    printf("LOL: %i", (*tree)->a);


}