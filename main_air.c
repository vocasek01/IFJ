#include "treegen.h"
// #include "treegen.c"

char *name[] = {
    "START",

    //Keyword or Identificator
    "IDENTIFICATOR", // text bez (if,else,for,return,package,func)
    "KEYWORD",       // if, else, for, return, package, func
    "IDENORKEY",     //

    //Type of date
    "INT", // celé číslo 42

    "DOUBLE",    // 4.2, nebo 4e+4.47
    "DOUB_DOT1", //
    "DOUB_DOT2", //
    "DOUB_EXP1", //
    "DOUB_EXP2",

    "STR",     // "text"
    "ESCAPE1", //
    "ESCAPE2", //

    "IFJ21", // "ifj21"

    "COLON", // :
    "COMMA", // ,

    //Opetators
    "ADD",     // +
    "SUB",     // -
    "MUL",     // *
    "DIV",     // /
    "INT_DIV", // //
    "LEN",     // #
    "GT",      // >
    "LT",      // <
    "NOTEQ",   // !=
    "GTE",     // >=
    "LTE",     // <=
    "EQ",      // ==
    "KONC",    // ..

    "ASSIGN", // =
    "RBR",    // )
    "LBR",    // (

    //Other
    "ENDOFFILE",    //EOF
    "EOL",          //EOL
    "COMMENT",      // --text
    "BLOCKCOMMENT", // --[ text ]
    "BLOCKORLINE",  // which comment

    //Lex. error
    "ERROR"
};

int main(int argc, char *argv[])
{
    argc = 2;
    argv[1] = "test_pr2.txt";
    scannerInit(argv[1]);
    Token token;
    do
    {
        token = getToken();
        printf("type: %s atribut: %s \n----------------------------------------------------------------------\n", name[token.type], token.attribute );
    }
    while(token.type != ENDOFFILE);


    // bst_node_t *tree;
    // tree = NULL;
    // tree = (bst_node_t *) malloc(sizeof(struct bst_node));
    // // tree->a = 0;
    // add_next(&tree);

    // printf("LOL: %i", tree->a);
}