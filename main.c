#include "treegen.h"
#include "parser.h"
#include "parser.c"

// #include "treegen.c"

int main(int argc, char *argv[])
{
    argc = 3;
    argv[1] = "test_pr2.txt";
    argv[2] = "out.txt";
    scannerInit(argv[1]);
    set_code_output(fopen(argv[2], "w"));
    start();
}
