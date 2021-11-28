#include <stdio.h>
#include <stdlib.h>
// #include "scanner.c"
// #include "scanner.h"
#include "codegen.c"
// #include "parser.h"
#include "parser.c"
// #include "parser.c"

// #include "treegen.c"

int main(int argc, char *argv[])
{
    argc = 3;
    argv[1] = "test_pr2.txt";
    argv[2] = "out.txt";
    scannerInit(argv[1]);
    set_code_output(fopen(argv[2], "w"));
    // generate_int2float();
    // generate_int2float();
    start();
    return 0;
}