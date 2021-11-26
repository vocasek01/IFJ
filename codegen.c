#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "string.h"
#include "symtable.h"

FILE *output;

int set_code_output(FILE *file)
{
    if (file == NULL)
    {
        return -1;
    }
    output = file;
    return 0;
}

int generate_header()
{
    fprintf(output, ".IFJcode20\n");
    fprintf(output, "JUMP $$main\n\n");
    return 0;
}

int generate_func_top(char *function_identifier)
{
    if (strcmp(function_identifier, "main") == 0)
    {
        fprintf(output, "LABEL $$main\n");
        fprintf(output, "CREATEFRAME\n");
    }
    else
    {
        fprintf(output, "LABEL $%s\n", function_identifier);
    }
    fprintf(output, "PUSHFRAME\n");
    return 0;
}

int generate_func_param(char *param_identifier, int param_pos)
{
    fprintf(output, "DEFVAR LF@%s\n", param_identifier);
    fprintf(output, "MOVE LF@%s LF@%%%d\n", param_identifier, param_pos);
    return 0;
}

int generate_func_bottom(char *function_identifier)
{
    if (strcmp(function_identifier, "main") == 0)
    {
        fprintf(output, "# end of main body\n");
        return 0;
    }
    fprintf(output, "POPFRAME\n");
    fprintf(output, "RETURN\n\n");
    return 0;
}

int generate_declaration(char *frame, char *identifier)
{
    fprintf(output, "DEFVAR %s%s\n", frame, identifier);
    return 0;
}

int generate_move(char *frame, char *identifier, char *source_frame, char *source)
{
    fprintf(output, "MOVE %s%s %s%s\n", frame, identifier, source_frame, source);
    return 0;
}

int generate_frame()
{
    fprintf(output, "CREATEFRAME\n");
    return 0;
}

int generate_function_call(char *func_name)
{
    fprintf(output, "CALL $%s\n", func_name);
    return 0;
}

int generate_push(char *frame, char *variable)
{
    fprintf(output, "PUSHS %s%s\n", frame, variable);
    return 0;
}