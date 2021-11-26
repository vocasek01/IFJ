#include <stdio.h>
#include "symtable.h"

int set_code_output(FILE *file);
int generate_header();
int generate_func_top(char *function_identifier);
int generate_func_param(char *param_identifier, int param_pos);
int generate_func_bottom(char *function_identifier);
int generate_move(char *dest_frame, char *identifier, char *source_frame, char *source);
int generate_return_move(char *identifier, int position);
int generate_declaration(char *dest_frame, char *identifier);
int generate_frame();
int generate_push(char *dest_frame, char *variable);
