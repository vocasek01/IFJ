#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "string.h"
// #include "scanner.h"
#include "scanner.c"
#include "parser.h"

// #include "symtable.h"
#pragma once

FILE *ifj_code;
int generate_inputs();
int generate_inputi();
int generate_inputf();
int generate_print();
int generate_int2float();
int generate_float2int();
int generate_len();
int generate_substr();
int generate_ord();
int generate_chr();
int generate_builtin_function(char *function_identifier);

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

    // char *id = "inputf";
    // generate_builtin_function(id);
    // id = "inputi";
    // generate_builtin_function(id);
    // id = "inputs";
    // generate_builtin_function(id);
    // id = "print";
    // generate_builtin_function(id);
    // id = "int2float";
    // generate_builtin_function(id);
    // id = "float2int";
    // generate_builtin_function(id);
    // id = "len";
    // generate_builtin_function(id);
    // id = "substr";
    // generate_builtin_function(id);
    // id = "ord";
    // generate_builtin_function(id);
    // id = "chr";
    // generate_builtin_function(id);
    return 0;
}

int generate_func_top(char *function_identifier)
{
    if (strcmp(function_identifier, "main") == 0)
    {
        fprintf(ifj_code, "LABEL $$main\n");
        fprintf(ifj_code, "CREATEFRAME\n");
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
    fprintf(ifj_code, "DEFVAR LF@%s\n", param_identifier);
    fprintf(ifj_code, "MOVE LF@%s LF@%%%d\n", param_identifier, param_pos);
    return 0;
}

int generate_return_values(typeVar type, int i)
{
    char *empty_value;
    char *ifjcode_type;
    switch (type)
    {
    // case VOID:
    //     ifjcode_type = "nil";
    //     empty_value = "nil";
    //     break;
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
    fprintf(ifj_code, "DEFVAR LF@%%retval%d\n", i);
    fprintf(ifj_code, "MOVE LF@%%retval%d %s@%s\n", i, ifjcode_type, empty_value);
    return 0;
}

int generate_func_bottom(char *function_identifier)
{
    if (strcmp(function_identifier, "main") == 0)
    {
        fprintf(ifj_code, "# end of main body\n");
        return 0;
    }
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n\n");
    return 0;
}

bool chceck_character_ok(int character)
{                                                                                          //
    if ((character >= 48 && character <= 57) || (character >= 65 && character <= 90) ||    //0-9, a-z
        (character >= 97 && character <= 122) || (character == 95) || (character == 45) || //A-Z, _, -
        (character >= 36 && character <= 38) || (character == 42) || (character == 33) ||  //$,%,&,*,!
        (character == 63))
    { //?
        return true;
    }
    else
    {
        return false;
    }
}

int generate_builtin_function(char *function_identifier)
{
    if (strcmp(function_identifier, "inputs") == 0)
    {
        return generate_inputs();
    }
    else if (strcmp(function_identifier, "inputi") == 0)
    {
        return generate_inputi();
    }
    else if (strcmp(function_identifier, "inputf") == 0)
    {
        return generate_inputf();
    }
    else if (strcmp(function_identifier, "print") == 0)
    {
        return generate_print();
    }
    else if (strcmp(function_identifier, "int2float") == 0)
    {
        // return generate_int2float();
    }
    else if (strcmp(function_identifier, "float2int") == 0)
    {
        return generate_float2int();
    }
    else if (strcmp(function_identifier, "len") == 0)
    {
        return generate_len();
    }
    else if (strcmp(function_identifier, "substr") == 0)
    {
        return generate_substr();
    }
    else if (strcmp(function_identifier, "ord") == 0)
    {
        return generate_ord();
    }
    else if (strcmp(function_identifier, "chr") == 0)
    {
        return generate_chr();
    }
    else
        return 99; //NO built-in function
}

int generate_inputs()
{
    fprintf(ifj_code, "LABEL $inputs\n");
    fprintf(ifj_code, "PUSHFRAME\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval1\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval2\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(ifj_code, "MOVE LF@%%retval2 int@0\n");
    fprintf(ifj_code, "DEFVAR LF@inputs\n");
    fprintf(ifj_code, "READ LF@inputs string\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 LF@inputs\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    return 0;
}

int generate_inputi()
{
    char *function_identifier = "inputi";
    fprintf(ifj_code, "LABEL $inputi\n");
    fprintf(ifj_code, "PUSHFRAME\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval1\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval2\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(ifj_code, "MOVE LF@%%retval2 int@0\n");
    fprintf(ifj_code, "DEFVAR LF@inputi\n");
    fprintf(ifj_code, "READ LF@inputi int\n");
    fprintf(ifj_code, "DEFVAR LF@helpertype\n");
    fprintf(ifj_code, "TYPE LF@helpertype LF@inputi\n");
    fprintf(ifj_code, "JUMPIFEQ $end_inputi LF@helpertype string@nil\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 LF@inputi\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    fprintf(ifj_code, "LABEL $end_inputi\n");
    fprintf(ifj_code, "MOVE LF@%%retval2 int@1\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    return 0;
}

int generate_inputf()
{
    fprintf(ifj_code, "LABEL $inputf\nPUSHFRAME\nDEFVAR LF@%%retval1\nDEFVAR LF@%%retval2\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 nil@nil\nMOVE LF@%%retval2 int@0\n");
    fprintf(ifj_code, "DEFVAR LF@inputf\n");
    fprintf(ifj_code, "READ LF@inputf float\n");
    fprintf(ifj_code, "DEFVAR LF@helpertype\n");
    fprintf(ifj_code, "TYPE LF@helpertype LF@inputf\n");
    fprintf(ifj_code, "JUMPIFEQ $end_inputf LF@helpertype string@nil\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 LF@inputf\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    fprintf(ifj_code, "LABEL $end_inputf\n");
    fprintf(ifj_code, "MOVE LF@%%retval2 int@1\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    return 0;
}

int generate_print()
{
    fprintf(ifj_code, "%s", "LABEL $print\nPUSHFRAME\nDEFVAR LF@counter\nMOVE LF@counter LF@%%1\n# while\nJUMP $$print_while_end\nLABEL $$print_while\nCREATEFRAME\nDEFVAR TF@variable\nPOPS TF@variable\nWRITE TF@variable\nSUB LF@counter LF@counter int@1\nLABEL $$print_while_end\nJUMPIFNEQ $$print_while LF@counter int@0\n# end while\nPOPFRAME\nRETURN\n\n");
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

int generate_len()
{
    fprintf(ifj_code, "LABEL $len\n");
    fprintf(ifj_code, "PUSHFRAME\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval1\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval2\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(ifj_code, "MOVE LF@%%retval2 int@0\n");
    fprintf(ifj_code, "DEFVAR LF@len\n");
    fprintf(ifj_code, "MOVE LF@len LF@%%1\n");
    fprintf(ifj_code, "STRLEN LF@%%retval1 LF@len\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    return 0;
}

int generate_substr()
{ 
    fprintf(ifj_code, "LABEL $substr\n");
    fprintf(ifj_code, "PUSHFRAME\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval1\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval2\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(ifj_code, "MOVE LF@%%retval2 int@0\n");
    fprintf(ifj_code, "DEFVAR LF@substr_string\n");
    fprintf(ifj_code, "MOVE LF@substr_string LF@%%1\n"); //read first param
    fprintf(ifj_code, "DEFVAR LF@substr_begin\n");
    fprintf(ifj_code, "MOVE LF@substr_begin LF@%%2\n"); //read second param
    fprintf(ifj_code, "DEFVAR LF@substr_size\n");
    fprintf(ifj_code, "MOVE LF@substr_size LF@%%3\n"); //read third param
    fprintf(ifj_code, "DEFVAR LF@LT1return\n");
    fprintf(ifj_code, "LT LF@LT1return LF@substr_begin int@0\n");
    fprintf(ifj_code, "JUMPIFEQ $substr_exit LF@LT1return bool@true\n"); //begin < 0
    fprintf(ifj_code, "DEFVAR LF@LT2return\n");
    fprintf(ifj_code, "LT LF@LT2return LF@substr_size int@0\n");
    fprintf(ifj_code, "JUMPIFEQ $substr_exit LF@LT2return bool@true\n"); //size < 0
    fprintf(ifj_code, "DEFVAR LF@LENreturn\n");
    fprintf(ifj_code, "STRLEN LF@LENreturn LF@substr_string\n");
    fprintf(ifj_code, "DEFVAR LF@GT1return\n");
    fprintf(ifj_code, "GT LF@GT1return LF@substr_begin LF@LENreturn\n");
    fprintf(ifj_code, "JUMPIFEQ $substr_exit LF@GT1return , function_identifierbool@true\n"); //begin>strlen
    fprintf(ifj_code, "DEFVAR LF@LENreturnMinusOne\n");
    fprintf(ifj_code, "SUB LF@LENreturnMinusOne LF@LENreturn int@1\n");
    fprintf(ifj_code, "DEFVAR LF@GT2return\n"); //size > (strlen(string) - 1)
    fprintf(ifj_code, "GT LF@GT2return LF@substr_size LF@LENreturnMinusOne\n");
    fprintf(ifj_code, "JUMPIFEQ $substr_size_bigger LF@GT2return bool@true\n");
    fprintf(ifj_code, "DEFVAR LF@cycle_counter\n");
    fprintf(ifj_code, "MOVE LF@cycle_counter LF@substr_size\n");
    fprintf(ifj_code, "DEFVAR LF@s_output\n");
    fprintf(ifj_code, "DEFVAR LF@stringonechar\n");
    fprintf(ifj_code, "GETCHAR LF@stringonechar LF@substr_string LF@substr_begin\n");
    fprintf(ifj_code, "ADD LF@substr_begin LF@substr_begin int@1\n");
    fprintf(ifj_code, "MOVE LF@s_output LF@stringonechar\n");
    fprintf(ifj_code, "SUB LF@cycle_counter LF@cycle_counter int@1\n");
    fprintf(ifj_code, "DEFVAR LF@concatenate\n");
    fprintf(ifj_code, "DEFVAR LF@next_round_result\n");
    fprintf(ifj_code, "GT LF@next_round_result LF@cycle_counter int@0\n");
    fprintf(ifj_code, "JUMPIFEQ $substr_cycle LF@next_round_result bool@true\n");
    fprintf(ifj_code, "JUMP $substr_end\n");

    fprintf(ifj_code, "LABEL $substr_cycle\n");
    fprintf(ifj_code, "GETCHAR LF@stringonechar LF@substr_string LF@substr_begin\n");
    fprintf(ifj_code, "CONCAT LF@concatenate LF@s_output LF@stringonechar\n");
    fprintf(ifj_code, "MOVE LF@s_output LF@concatenate\n");
    fprintf(ifj_code, "ADD LF@substr_begin LF@substr_begin int@1\n");
    fprintf(ifj_code, "SUB LF@cycle_counter LF@cycle_counter int@1\n");
    fprintf(ifj_code, "GT LF@next_round_result LF@cycle_counter int@0\n");
    fprintf(ifj_code, "JUMPIFEQ $substr_cycle LF@next_round_result bool@true\n");
    fprintf(ifj_code, "JUMP $substr_end\n");

    fprintf(ifj_code, "LABEL $substr_size_bigger\n");
    fprintf(ifj_code, "DEFVAR LF@helper\n");
    fprintf(ifj_code, "MOVE LF@helper LF@LENreturn\n");
    fprintf(ifj_code, "DEFVAR LF@LENminusBegin\n");
    fprintf(ifj_code, "SUB LF@LENminusBegin LF@LENreturn LF@substr_begin\n");
    fprintf(ifj_code, "MOVE LF@substr_size LF@LENminusBegin\n"); //size=len(str)-begin
    fprintf(ifj_code, "DEFVAR LF@cycle_counter\n");
    fprintf(ifj_code, "MOVE LF@cycle_counter LF@substr_size\n");
    fprintf(ifj_code, "DEFVAR LF@s_output\n");
    fprintf(ifj_code, "DEFVAR LF@stringonechar\n");
    fprintf(ifj_code, "GETCHAR LF@stringonechar LF@substr_string LF@substr_begin\n");
    fprintf(ifj_code, "ADD LF@substr_begin LF@substr_begin int@1\n");
    fprintf(ifj_code, "MOVE LF@s_output LF@stringonechar\n");
    fprintf(ifj_code, "SUB LF@cycle_counter LF@cycle_counter int@1\n");
    fprintf(ifj_code, "DEFVAR LF@concatenate\n");
    fprintf(ifj_code, "DEFVAR LF@next_round_result\n");
    fprintf(ifj_code, "GT LF@next_round_result LF@cycle_counter int@0\n");
    fprintf(ifj_code, "JUMPIFEQ $substr_cycle LF@next_round_result bool@true\n");
    fprintf(ifj_code, "JUMP $substr_end\n");
    fprintf(ifj_code, "LABEL $substr_exit\n"); //NEW LABEL EXIT
    fprintf(ifj_code, "MOVE LF@%%retval2 int@1\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    fprintf(ifj_code, "LABEL $substr_end\n"); //NEW LABEL END
    fprintf(ifj_code, "MOVE LF@%%retval1 LF@s_output\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    return 0;
}

int generate_ord()
{ //string=LF@%1, number=LF@%2
    fprintf(ifj_code, "LABEL $ord_exit\n");
    fprintf(ifj_code, "MOVE LF@%%retval2 int@1\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    fprintf(ifj_code, "LABEL $ord\n");
    fprintf(ifj_code, "PUSHFRAME\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval1\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval2\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(ifj_code, "MOVE LF@%%retval2 int@0\n");
    fprintf(ifj_code, "DEFVAR LF@ord1\n");
    fprintf(ifj_code, "MOVE LF@ord1 LF@%%1\n");
    fprintf(ifj_code, "DEFVAR LF@ord2\n");
    fprintf(ifj_code, "MOVE LF@ord2 LF@%%2\n");
    fprintf(ifj_code, "DEFVAR LF@LTreturn\n");
    fprintf(ifj_code, "LT LF@LTreturn LF@ord2 int@0\n");
    fprintf(ifj_code, "JUMPIFEQ $ord_exit LF@LTreturn bool@true\n");
    fprintf(ifj_code, "DEFVAR LF@LENreturn\n");
    fprintf(ifj_code, "STRLEN LF@LENreturn LF@ord1\n");
    fprintf(ifj_code, "DEFVAR LF@LENminusONE\n");
    fprintf(ifj_code, "SUB LF@LENminusONE LF@LENreturn int@1\n");
    fprintf(ifj_code, "DEFVAR LF@GTreturn\n");
    fprintf(ifj_code, "GT LF@GTreturn LF@ord2 LF@LENminusONE\n");
    fprintf(ifj_code, "JUMPIFEQ $ord_exit LF@GTreturn bool@true\n");
    fprintf(ifj_code, "STRI2INT LF@%%retval1 LF@ord1 LF@ord2\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    return 0;
}

int generate_chr()
{
    char *function_identifier = "chr";
    fprintf(ifj_code, "LABEL $%s_exit\n", function_identifier);
    fprintf(ifj_code, "MOVE LF@%%retval2 int@1\n");
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
    fprintf(ifj_code, "LABEL $%s\n", function_identifier);
    fprintf(ifj_code, "PUSHFRAME\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval1\n");
    fprintf(ifj_code, "DEFVAR LF@%%retval2\n");
    fprintf(ifj_code, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(ifj_code, "MOVE LF@%%retval2 int@0\n");
    fprintf(ifj_code, "DEFVAR LF@%s%d\n", function_identifier, 1);
    fprintf(ifj_code, "MOVE LF@%s%d LF@%%1\n", function_identifier, 1);
    fprintf(ifj_code, "DEFVAR LF@GTreturn\n"); // test number > 255 || number < 0
    fprintf(ifj_code, "DEFVAR LF@LTreturn\n");
    fprintf(ifj_code, "LT LF@LTreturn LF@%s%d int@0\n", function_identifier, 1);
    fprintf(ifj_code, "GT LF@GTreturn LF@%s%d int@255\n", function_identifier, 1);
    fprintf(ifj_code, "JUMPIFEQ $%s_exit LF@LTreturn bool@true\n", function_identifier);
    fprintf(ifj_code, "JUMPIFEQ $%s_exit LF@GTreturn bool@true\n", function_identifier);
    fprintf(ifj_code, "INT2CHAR LF@%%retval1 LF@%s%d\n", function_identifier, 1);
    fprintf(ifj_code, "POPFRAME\n");
    fprintf(ifj_code, "RETURN\n");
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
    case ADD:
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
        fprintf(ifj_code, "EQ LF@%s LF@%s LF@%s\n", result, a, b);
        fprintf(ifj_code, "NOT LF@%s LF@%s\n", result, result);
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
    fprintf(ifj_code, "# True%d\n", number);
    return 0;
}

int generate_if_middle(int number)
{
    fprintf(ifj_code, "JUMP $$IF_END%d\n", number);
    fprintf(ifj_code, "LABEL $$IF_FALSE%d\n", number);
    fprintf(ifj_code, "# False%d\n", number);
    return 0;
}

int generate_if_end(int number)
{
    fprintf(ifj_code, "LABEL $$IF_END%d\n", number);
    fprintf(ifj_code, "# if%d end\n", number);
    return 0;
}

int generate_while_head(char *check_operand_identifier, int while_counter)
{
    fprintf(ifj_code, "JUMPIFEQ $CYCLE%d LF@%s bool@true\n", while_counter, check_operand_identifier);
    fprintf(ifj_code, "JUMP $END%d\n", while_counter);
    return 0;
}

int generate_while_label_cycle(int while_counter)
{
    fprintf(ifj_code, "LABEL $CYCLE%d\n", while_counter);
    return 0;
}

int generate_while_iterate(char *check_operand_identifier, int while_counter)
{
    fprintf(ifj_code, "JUMPIFEQ $CYCLE%d LF@%s bool@true\n", while_counter, check_operand_identifier);
    fprintf(ifj_code, "JUMP $END%d\n", while_counter);
    return 0;
}

int generate_while_label_end(int while_counter)
{
    fprintf(ifj_code, "LABEL $END%d\n", while_counter);
    return 0;
}