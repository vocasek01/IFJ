#include <stdio.h>
#include <stdlib.h>
#include "codegen.c"
#include "parser.c"


int main(int argc, char *argv[])
{
    scannerInit(stdin);
    set_code_output(stdout);
    
    // argc = 3;
    // argv[1] = "test_pr2.in";
    // argv[2] = "out.out";

    // scannerInit(fopen(argv[1], "r"));
    // set_code_output(fopen(argv[2], "w"));

    int a = start();
    // printf("EROR CODE:%d\n", a);
    return a;

}
