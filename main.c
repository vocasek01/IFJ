#include <stdio.h>
#include <stdlib.h>
#include "codegen.c"
#include "parser.c"


int main(int argc, char *argv[])
{
    scannerInit(stdin);
    set_code_output(stdout);
    return start();
}