#include <stdio.h>
#include <stdlib.h>
// #include "codegen.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    scannerInit(stdin);
    set_code_output(stdout);
    return start();
}