/*------------------------------------------
 Title:  codegen.c
 Author: Kirillov Artem
 Login:  xkiril01
 Date:   23 Nov 2021;
------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "string.h"
#include "scanner.c"
#include "parser.h"
#pragma once

#pragma once

FILE *ifj_code;

int set_code_output(FILE *file)
{
    if (file == NULL)
    {
        return -1;
    }
    ifj_code = file;
    return 0;
}

int generate_header()
{
    fprintf(ifj_code, ".IFJcode21\n");
    fprintf(ifj_code, "JUMP $$main\n\n");
    return 0;
}

int generate_func_top(char *function_identifier)
{
    if (strcmp(function_identifier, "main") == 0)
    {
        fprintf(ifj_code, "LABEL $$main\nCREATEFRAME\n");
    }
    else
    {
        fprintf(ifj_code, "LABEL $%s\n", function_identifier);
    }
    fprintf(ifj_code, "PUSHFRAME\n");
    return 0;
}

int generate_func_param(char *param_identifier, int param_pos)
{
    fprintf(ifj_code, "DEFVAR LF@%s\nMOVE LF@%s LF@%%%d\n", param_identifier, param_identifier, param_pos);
    return 0;
}

int generate_return_values(typeVar type, int i)
{
    char *empty_value;
    char *ifjcode_type;
    switch (type)
    {
    case sSTR:
        ifjcode_type = "string";
        empty_value = "\"\"";
        break;
    case sINT:
        ifjcode_type = "int";
        empty_value = "0";
        break;
    case FLOAT:
        ifjcode_type = "float";
        empty_value = "0x0.0p+0";
        break;
    }
    fprintf(ifj_code, "DEFVAR LF@%%retval%d\nMOVE LF@%%retval%d %s@%s\n", i, i, ifjcode_type, empty_value);
    return 0;
}

int generate_func_bottom(char *function_identifier)
{
    if (strcmp(function_identifier, "main") == 0)
    {
        return 0;
    }
    fprintf(ifj_code, "POPFRAME\nRETURN\n\n");
    return 0;
}

int generate_int2float(char* identificator)
{
    fprintf(ifj_code, "INT2FLOAT LF@%s LF@%s\n",identificator,identificator);
    return 0;
}

int generate_float2int()
{
    fprintf(ifj_code, "LABEL $float2int\nPUSHFRAME\nDEFVAR LF@%%retval1\nMOVE LF@%%retval1 nil@nil\n");
    fprintf(ifj_code, "DEFVAR LF@float2int\nMOVE LF@float2int LF@%%1\nFLOAT2INT LF@%%retval1 LF@float2int\n");
    fprintf(ifj_code, "POPFRAME\nRETURN\n");
    return 0;
}

int generate_len(char* a, char* b)
{
    
    fprintf(ifj_code, "STRLEN LF@%s LF@%s\n",a,b);
    
    return 0;
}

int generate_write(char *dest_frame,char *var)
{
    fprintf(ifj_code, "WRITE %s%s\n", dest_frame, var);
    return 0;
}

int generate_readi(char *dest_frame, char *var, char *type)
{
    fprintf(ifj_code, "READ %s%s %s\n", dest_frame, var, type);
    return 0;
}

int generate_declaration(char *dest_frame, char *identifier)
{
    fprintf(ifj_code, "DEFVAR %s%s\n", dest_frame, identifier);
    return 0;
}

int generate_move(char *dest_frame, char *identifier, char *source_frame, char *source)
{
    fprintf(ifj_code, "MOVE %s%s %s%s\n", dest_frame, identifier, source_frame, source);
    return 0;
}

int generate_frame()
{
    fprintf(ifj_code, "CREATEFRAME\n");
    return 0;
}

int generate_function_call(char *function_name)
{
    fprintf(ifj_code, "CALL $%s\n", function_name);
    return 0;
}

int generate_push(char *dest_frame, char *variable)
{
    fprintf(ifj_code, "PUSHS %s%s\n", dest_frame, variable);
    return 0;
}

int generate_return_move(char *identifier, int position)
{
    fprintf(ifj_code, "MOVE LF@%%retval%d %s%s\n", position, "LF@", identifier);
    return 0;
}

int generate_arithmetic_operation_string(Token operation, char *result, char *a, char *b)
{
    switch (operation.type)
    {
    case KONC:
        fprintf(ifj_code, "CONCAT LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case LT:
        fprintf(ifj_code, "LT LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case LTE:
        fprintf(ifj_code, "GT LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    case EQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case NOTEQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    case GT:
        fprintf(ifj_code, "GT LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case GTE:
        fprintf(ifj_code, "LT LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    default:
        fprintf(stderr, "Unsupported operation!");
        return 1;
    }
    return 0;
}

int generate_arithmetic_operation_bool(Token operation, char *result, char *a, char *b)
{
    switch (operation.type)
    {
    case LT:
        fprintf(ifj_code, "LT LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case LTE:
        fprintf(ifj_code, "GT LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    case EQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case NOTEQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    case GT:
        fprintf(ifj_code, "GT LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case GTE:
        fprintf(ifj_code, "LT LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    default:
        fprintf(stderr, "Unsupported operation!");
        return 1;
    }
    return 0;
}

int generate_arithmetic_operation_int(Token operation, char *result, char *a, char *b)
{
    switch (operation.type)
    {
    case ADD:
        fprintf(ifj_code, "ADD LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case SUB:
        fprintf(ifj_code, "SUB LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case MUL:
        fprintf(ifj_code, "MUL LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case DIV:
        fprintf(ifj_code, "DIV LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case INT_DIV:
        fprintf(ifj_code, "IDIV LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case LT:
        fprintf(ifj_code, "LT LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case LTE:
        fprintf(ifj_code, "GT LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    case EQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case NOTEQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    case GT:
        fprintf(ifj_code, "GT LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case GTE:
        fprintf(ifj_code, "LT LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    default:
        fprintf(stderr, "Unsupported operation!");
        return 1;
    }
    return 0;
}

int generate_arithmetic_operation_float(Token operation, char *result, char *a, char *b)
{
    switch (operation.type)
    {
    case ADD:
        fprintf(ifj_code, "ADD LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case SUB:
        fprintf(ifj_code, "SUB LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case MUL:
        fprintf(ifj_code, "MUL LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case DIV:
        fprintf(ifj_code, "DIV LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case LT:
        fprintf(ifj_code, "LT LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case LTE:
        fprintf(ifj_code, "GT LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    case EQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case NOTEQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    case GT:
        fprintf(ifj_code, "GT LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case GTE:
        fprintf(ifj_code, "LT LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
        break;
    default:
        fprintf(stderr, "Unsupported operation!");
        return 1;
    }
    return 0;
}

int generate_arithmetic_operation_nil(Token operation, char *result, char *a, char *b)
{
    switch (operation.type)
    {
    case EQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        break;
    case NOTEQ:
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\nNOT LF@%s LF@%s\n", result, a, b, result, result);
        break;
    default:
        fprintf(stderr, "Unsupported operation!");
        return ERR_NIL_UNSUPPORTED_OPERATION;
    }
    return 0;
}

int generate_if_head(char *compare_variable, int number)
{
    fprintf(ifj_code, "JUMPIFEQ $$IF_FALSE%d LF@%s bool@false\n", number, compare_variable);
    return 0;
}

int generate_if_middle(int number)
{
    fprintf(ifj_code, "JUMP $$IF_END%d\n", number);
    fprintf(ifj_code, "LABEL $$IF_FALSE%d\n", number);
    return 0;
}

int generate_if_end(int number)
{
    fprintf(ifj_code, "LABEL $$IF_END%d\n", number);
    return 0;
}

int generate_while_head(char *check_operand_identifier, int while_counter)
{
    fprintf(ifj_code, "JUMPIFEQ $CYCLE%d LF@%s bool@true\nJUMP $END%d\n", while_counter, check_operand_identifier, while_counter);
    return 0;
}

int generate_while_label_cycle(int while_counter)
{
    fprintf(ifj_code, "LABEL $CYCLE%d\n", while_counter);
    return 0;
}

int generate_while_iterate(char *check_operand_identifier, int while_counter)
{
    fprintf(ifj_code, "JUMPIFEQ $CYCLE%d LF@%s bool@true\nJUMP $END%d\n", while_counter, check_operand_identifier, while_counter);
    return 0;
}

int generate_while_label_end(int while_counter)
{
    fprintf(ifj_code, "LABEL $END%d\n", while_counter);
    return 0;
}
