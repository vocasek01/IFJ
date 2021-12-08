/*------------------------------------------
 Title:  codegen.h
 Author: Kirillov Artem
 Login:  xkiril01
 Date:   23 Nov 2021;
------------------------------------------*/

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
int generate_function_call(char *function_name);
int generate_arithmetic_operation_string(Token operation, char *result, char *a, char *b);
int generate_arithmetic_operation_int(Token operation, char *result, char *a, char *b);
int generate_arithmetic_operation_float(Token operation, char *result, char *a, char *b);
int generate_arithmetic_operation_nil(Token operation, char *result, char *a, char *b);
int generate_arithmetic_operation_bool(Token operation, char *result, char *a, char *b);
int generate_if_head(char *compare_variable, int number);
int generate_if_middle(int number);
int generate_if_end(int number);
int generate_while_head(char *check_operand_identifier, int for_counter);
int generate_while_label_end(int for_counter);
int generate_while_label_cycle(int for_counter);
int generate_while_iterate(char *check_operand_identifier, int for_counter);
int generate_len(char* a, char* b);
