#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "string.h"
#include "symtable.h"

FILE *code_output_file;
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
    code_output_file = file;
    return 0;
}

int generate_header()
{
    fprintf(code_output_file, ".IFJcode20\n");
    fprintf(code_output_file, "JUMP $$main\n\n");

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
        fprintf(code_output_file, "LABEL $$main\n");
        fprintf(code_output_file, "CREATEFRAME\n");
    }
    else
    {
        fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    }
    fprintf(code_output_file, "PUSHFRAME\n");
    return 0;
}

int generate_func_param(char *param_identifier, int param_pos)
{
    fprintf(code_output_file, "DEFVAR LF@%s\n", param_identifier);
    fprintf(code_output_file, "MOVE LF@%s LF@%%%d\n", param_identifier, param_pos);
    return 0;
}

int generate_return_values(Symtable_item *function)
{
    char *ifjcode_type;
    char *empty_value;
    for (int i = 0; i < function->return_values_count; i++)
    {
        switch (function->dataType[i])
        {
        case DT_VOID:
            ifjcode_type = "nil";
            empty_value = "nil";
            break;
        case DT_STRING:
            ifjcode_type = "string";
            empty_value = "\"\"";
            break;
        case DT_INT:
            ifjcode_type = "int";
            empty_value = "0";
            break;
        case DT_FLOAT:
            ifjcode_type = "float";
            empty_value = "0x0.0p+0";
            break;
        }
        fprintf(code_output_file, "%s%d%s", "DEFVAR LF@%retval", i + 1, "\n");
        fprintf(code_output_file, "MOVE LF@%%retval%d %s@%s\n", i + 1, ifjcode_type, empty_value);
    }
    return 0;
}

int generate_func_bottom(char *function_identifier)
{
    if (strcmp(function_identifier, "main") == 0)
    {
        fprintf(code_output_file, "# end of main body\n");
        return 0;
    }
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n\n");
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
        return generate_int2float();
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
    char *function_identifier = "inputs";
    fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    fprintf(code_output_file, "PUSHFRAME\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval1\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval2\n");
    fprintf(code_output_file, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(code_output_file, "MOVE LF@%%retval2 int@0\n");
    fprintf(code_output_file, "DEFVAR LF@%s\n", function_identifier);
    fprintf(code_output_file, "READ LF@%s string\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%%retval1 LF@%s\n", function_identifier);
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    return 0;
}

int generate_inputi()
{
    char *function_identifier = "inputi";
    fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    fprintf(code_output_file, "PUSHFRAME\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval1\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval2\n");
    fprintf(code_output_file, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(code_output_file, "MOVE LF@%%retval2 int@0\n");
    fprintf(code_output_file, "DEFVAR LF@%s\n", function_identifier);
    fprintf(code_output_file, "READ LF@%s int\n", function_identifier);
    fprintf(code_output_file, "DEFVAR LF@helpertype\n");
    fprintf(code_output_file, "TYPE LF@helpertype LF@%s\n", function_identifier);
    fprintf(code_output_file, "JUMPIFEQ $end_%s LF@helpertype string@nil\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%%retval1 LF@%s\n", function_identifier);
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    fprintf(code_output_file, "LABEL $end_%s\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%%retval2 int@1\n");
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    return 0;
}

int generate_inputf()
{
    char *function_identifier = "inputf";
    fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    fprintf(code_output_file, "PUSHFRAME\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval1\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval2\n");
    fprintf(code_output_file, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(code_output_file, "MOVE LF@%%retval2 int@0\n");
    fprintf(code_output_file, "DEFVAR LF@%s\n", function_identifier);
    fprintf(code_output_file, "READ LF@%s float\n", function_identifier);
    fprintf(code_output_file, "DEFVAR LF@helpertype\n");
    fprintf(code_output_file, "TYPE LF@helpertype LF@%s\n", function_identifier);
    fprintf(code_output_file, "JUMPIFEQ $end_%s LF@helpertype string@nil\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%%retval1 LF@%s\n", function_identifier);
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    fprintf(code_output_file, "LABEL $end_%s\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%%retval2 int@1\n");
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    return 0;
}

int generate_print()
{
    fprintf(code_output_file, "%s", "LABEL $print\nPUSHFRAME\nDEFVAR LF@counter\nMOVE LF@counter LF@%1\n# while\nJUMP $$print_while_end\nLABEL $$print_while\nCREATEFRAME\nDEFVAR TF@variable\nPOPS TF@variable\nWRITE TF@variable\nSUB LF@counter LF@counter int@1\nLABEL $$print_while_end\nJUMPIFNEQ $$print_while LF@counter int@0\n# end while\nPOPFRAME\nRETURN\n\n");
    return 0;
}

int generate_int2float()
{
    char *function_identifier = "int2float";
    fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    fprintf(code_output_file, "PUSHFRAME\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval1\n");
    fprintf(code_output_file, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(code_output_file, "DEFVAR LF@%s\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%s LF@%%1\n", function_identifier);
    fprintf(code_output_file, "INT2FLOAT LF@%%retval1 LF@%s\n", function_identifier);
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    return 0;
}

int generate_float2int()
{
    char *function_identifier = "float2int";
    fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    fprintf(code_output_file, "PUSHFRAME\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval1\n");
    fprintf(code_output_file, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(code_output_file, "DEFVAR LF@%s\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%s LF@%%1\n", function_identifier);
    fprintf(code_output_file, "FLOAT2INT LF@%%retval1 LF@%s\n", function_identifier);
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    return 0;
}

int generate_len()
{
    char *function_identifier = "len";
    fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    fprintf(code_output_file, "PUSHFRAME\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval1\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval2\n");
    fprintf(code_output_file, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(code_output_file, "MOVE LF@%%retval2 int@0\n");
    fprintf(code_output_file, "DEFVAR LF@%s\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%s LF@%%1\n", function_identifier);
    fprintf(code_output_file, "STRLEN LF@%%retval1 LF@%s\n", function_identifier);
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    return 0;
}

int generate_substr()
{ 
    char *function_identifier = "substr";
    fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    fprintf(code_output_file, "PUSHFRAME\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval1\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval2\n");
    fprintf(code_output_file, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(code_output_file, "MOVE LF@%%retval2 int@0\n");
    fprintf(code_output_file, "DEFVAR LF@%s_string\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%s_string LF@%%1\n", function_identifier); //read first param
    fprintf(code_output_file, "DEFVAR LF@%s_begin\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%s_begin LF@%%2\n", function_identifier); //read second param
    fprintf(code_output_file, "DEFVAR LF@%s_size\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%s_size LF@%%3\n", function_identifier); //read third param
    fprintf(code_output_file, "DEFVAR LF@LT1return\n");
    fprintf(code_output_file, "LT LF@LT1return LF@%s_begin int@0\n", function_identifier);
    fprintf(code_output_file, "JUMPIFEQ $%s_exit LF@LT1return bool@true\n", function_identifier); //begin < 0
    fprintf(code_output_file, "DEFVAR LF@LT2return\n");
    fprintf(code_output_file, "LT LF@LT2return LF@%s_size int@0\n", function_identifier);
    fprintf(code_output_file, "JUMPIFEQ $%s_exit LF@LT2return bool@true\n", function_identifier); //size < 0
    fprintf(code_output_file, "DEFVAR LF@LENreturn\n");
    fprintf(code_output_file, "STRLEN LF@LENreturn LF@%s_string\n", function_identifier);
    fprintf(code_output_file, "DEFVAR LF@GT1return\n");
    fprintf(code_output_file, "GT LF@GT1return LF@%s_begin LF@LENreturn\n", function_identifier);
    fprintf(code_output_file, "JUMPIFEQ $%s_exit LF@GT1return bool@true\n", function_identifier); //begin>strlen
    fprintf(code_output_file, "DEFVAR LF@LENreturnMinusOne\n");
    fprintf(code_output_file, "SUB LF@LENreturnMinusOne LF@LENreturn int@1\n");
    fprintf(code_output_file, "DEFVAR LF@GT2return\n"); //size > (strlen(string) - 1)
    fprintf(code_output_file, "GT LF@GT2return LF@%s_size LF@LENreturnMinusOne\n", function_identifier);
    fprintf(code_output_file, "JUMPIFEQ $%s_size_bigger LF@GT2return bool@true\n", function_identifier);
    fprintf(code_output_file, "DEFVAR LF@cycle_counter\n");
    fprintf(code_output_file, "MOVE LF@cycle_counter LF@%s_size\n", function_identifier);
    fprintf(code_output_file, "DEFVAR LF@s_output\n");
    fprintf(code_output_file, "DEFVAR LF@stringonechar\n");
    fprintf(code_output_file, "GETCHAR LF@stringonechar LF@%s_string LF@%s_begin\n", function_identifier,
            function_identifier);
    fprintf(code_output_file, "ADD LF@%s_begin LF@%s_begin int@1\n", function_identifier, function_identifier);
    fprintf(code_output_file, "MOVE LF@s_output LF@stringonechar\n");
    fprintf(code_output_file, "SUB LF@cycle_counter LF@cycle_counter int@1\n");
    fprintf(code_output_file, "DEFVAR LF@concatenate\n");
    fprintf(code_output_file, "DEFVAR LF@next_round_result\n");
    fprintf(code_output_file, "GT LF@next_round_result LF@cycle_counter int@0\n");
    fprintf(code_output_file, "JUMPIFEQ $%s_cycle LF@next_round_result bool@true\n", function_identifier);
    fprintf(code_output_file, "JUMP $substr_end\n");

    fprintf(code_output_file, "LABEL $%s_cycle\n", function_identifier);
    fprintf(code_output_file, "GETCHAR LF@stringonechar LF@substr_string LF@substr_begin\n");
    fprintf(code_output_file, "CONCAT LF@concatenate LF@s_output LF@stringonechar\n");
    fprintf(code_output_file, "MOVE LF@s_output LF@concatenate\n");
    fprintf(code_output_file, "ADD LF@substr_begin LF@substr_begin int@1\n");
    fprintf(code_output_file, "SUB LF@cycle_counter LF@cycle_counter int@1\n");
    fprintf(code_output_file, "GT LF@next_round_result LF@cycle_counter int@0\n");
    fprintf(code_output_file, "JUMPIFEQ $substr_cycle LF@next_round_result bool@true\n");
    fprintf(code_output_file, "JUMP $substr_end\n");

    fprintf(code_output_file, "LABEL $%s_size_bigger\n", function_identifier);
    fprintf(code_output_file, "DEFVAR LF@helper\n");
    fprintf(code_output_file, "MOVE LF@helper LF@LENreturn\n");
    fprintf(code_output_file, "DEFVAR LF@LENminusBegin\n");
    fprintf(code_output_file, "SUB LF@LENminusBegin LF@LENreturn LF@substr_begin\n");
    fprintf(code_output_file, "MOVE LF@substr_size LF@LENminusBegin\n"); //size=len(str)-begin
    fprintf(code_output_file, "DEFVAR LF@cycle_counter\n");
    fprintf(code_output_file, "MOVE LF@cycle_counter LF@%s_size\n", function_identifier);
    fprintf(code_output_file, "DEFVAR LF@s_output\n");
    fprintf(code_output_file, "DEFVAR LF@stringonechar\n");
    fprintf(code_output_file, "GETCHAR LF@stringonechar LF@%s_string LF@%s_begin\n", function_identifier,
            function_identifier);
    fprintf(code_output_file, "ADD LF@%s_begin LF@%s_begin int@1\n", function_identifier, function_identifier);
    fprintf(code_output_file, "MOVE LF@s_output LF@stringonechar\n");
    fprintf(code_output_file, "SUB LF@cycle_counter LF@cycle_counter int@1\n");
    fprintf(code_output_file, "DEFVAR LF@concatenate\n");
    fprintf(code_output_file, "DEFVAR LF@next_round_result\n");
    fprintf(code_output_file, "GT LF@next_round_result LF@cycle_counter int@0\n");
    fprintf(code_output_file, "JUMPIFEQ $%s_cycle LF@next_round_result bool@true\n", function_identifier);
    fprintf(code_output_file, "JUMP $substr_end\n");
    fprintf(code_output_file, "LABEL $%s_exit\n", function_identifier); //NEW LABEL EXIT
    fprintf(code_output_file, "MOVE LF@%%retval2 int@1\n");
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    fprintf(code_output_file, "LABEL $%s_end\n", function_identifier); //NEW LABEL END
    fprintf(code_output_file, "MOVE LF@%%retval1 LF@s_output\n");
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    return 0;
}

int generate_ord()
{ //string=LF@%1, number=LF@%2
    char *function_identifier = "ord";
    fprintf(code_output_file, "LABEL $%s_exit\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%%retval2 int@1\n");
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    fprintf(code_output_file, "PUSHFRAME\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval1\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval2\n");
    fprintf(code_output_file, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(code_output_file, "MOVE LF@%%retval2 int@0\n");
    fprintf(code_output_file, "DEFVAR LF@%s%d\n", function_identifier, 1);
    fprintf(code_output_file, "MOVE LF@%s%d LF@%%1\n", function_identifier, 1);
    fprintf(code_output_file, "DEFVAR LF@%s%d\n", function_identifier, 2);
    fprintf(code_output_file, "MOVE LF@%s%d LF@%%2\n", function_identifier, 2);
    fprintf(code_output_file, "DEFVAR LF@LTreturn\n");
    fprintf(code_output_file, "LT LF@LTreturn LF@%s%d int@0\n", function_identifier, 2);
    fprintf(code_output_file, "JUMPIFEQ $%s_exit LF@LTreturn bool@true\n", function_identifier);
    fprintf(code_output_file, "DEFVAR LF@LENreturn\n");
    fprintf(code_output_file, "STRLEN LF@LENreturn LF@%s%d\n", function_identifier, 1);
    fprintf(code_output_file, "DEFVAR LF@LENminusONE\n");
    fprintf(code_output_file, "SUB LF@LENminusONE LF@LENreturn int@1\n");
    fprintf(code_output_file, "DEFVAR LF@GTreturn\n");
    fprintf(code_output_file, "GT LF@GTreturn LF@%s%d LF@LENminusONE\n", function_identifier, 2);
    fprintf(code_output_file, "JUMPIFEQ $%s_exit LF@GTreturn bool@true\n", function_identifier);
    fprintf(code_output_file, "STRI2INT LF@%%retval1 LF@%s%d LF@%s%d\n", function_identifier, 1, function_identifier,
            2);
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    return 0;
}

int generate_chr()
{
    char *function_identifier = "chr";
    fprintf(code_output_file, "LABEL $%s_exit\n", function_identifier);
    fprintf(code_output_file, "MOVE LF@%%retval2 int@1\n");
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    fprintf(code_output_file, "LABEL $%s\n", function_identifier);
    fprintf(code_output_file, "PUSHFRAME\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval1\n");
    fprintf(code_output_file, "DEFVAR LF@%%retval2\n");
    fprintf(code_output_file, "MOVE LF@%%retval1 nil@nil\n");
    fprintf(code_output_file, "MOVE LF@%%retval2 int@0\n");
    fprintf(code_output_file, "DEFVAR LF@%s%d\n", function_identifier, 1);
    fprintf(code_output_file, "MOVE LF@%s%d LF@%%1\n", function_identifier, 1);
    fprintf(code_output_file, "DEFVAR LF@GTreturn\n"); // test number > 255 || number < 0
    fprintf(code_output_file, "DEFVAR LF@LTreturn\n");
    fprintf(code_output_file, "LT LF@LTreturn LF@%s%d int@0\n", function_identifier, 1);
    fprintf(code_output_file, "GT LF@GTreturn LF@%s%d int@255\n", function_identifier, 1);
    fprintf(code_output_file, "JUMPIFEQ $%s_exit LF@LTreturn bool@true\n", function_identifier);
    fprintf(code_output_file, "JUMPIFEQ $%s_exit LF@GTreturn bool@true\n", function_identifier);
    fprintf(code_output_file, "INT2CHAR LF@%%retval1 LF@%s%d\n", function_identifier, 1);
    fprintf(code_output_file, "POPFRAME\n");
    fprintf(code_output_file, "RETURN\n");
    return 0;
}

int generate_declaration(char *dest_frame, char *identifier)
{
    fprintf(code_output_file, "DEFVAR %s%s\n", dest_frame, identifier);
    return 0;
}

int generate_move(char *dest_frame, char *identifier, char *source_frame, char *source)
{
    fprintf(code_output_file, "MOVE %s%s %s%s\n", dest_frame, identifier, source_frame, source);
    return 0;
}

int generate_frame()
{
    fprintf(code_output_file, "CREATEFRAME\n");
    return 0;
}

int generate_function_call(char *function_name)
{
    fprintf(code_output_file, "CALL $%s\n", function_name);
    return 0;
}

int generate_push(char *dest_frame, char *variable)
{
    fprintf(code_output_file, "PUSHS %s%s\n", dest_frame, variable);
    return 0;
}

int generate_return_move(char *identifier, int position)
{
    fprintf(code_output_file, "MOVE LF@%%retval%d %s%s\n", position, "LF@", identifier);
    return 0;
}

int generate_arithmetic_operation_string(Token_type operation, char *result, char *lhs, char *rhs)
{
    switch (operation)
    {
    case TT_PLUS:
        fprintf(code_output_file, "CONCAT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_LESS:
        fprintf(code_output_file, "LT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_LESS_OR_EQUALS:
        fprintf(code_output_file, "GT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        fprintf(code_output_file, "NOT LF@%s LF@%s\n", result, result);
        break;
    case TT_EQUALS:
        fprintf(code_output_file, "EQ LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_NOT_EQUALS:
        fprintf(code_output_file, "EQ LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        fprintf(code_output_file, "NOT LF@%s LF@%s\n", result, result);
        break;
    case TT_GREATER:
        fprintf(code_output_file, "GT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_GREATER_OR_EQUALS:
        fprintf(code_output_file, "LT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        fprintf(code_output_file, "NOT LF@%s LF@%s\n", result, result);
        break;
    default:
        fprintf(stderr, "Unsupported operation!");
        return 1;
    }
    return 0;
}

int generate_arithmetic_operation_int(Token_type operation, char *result, char *lhs, char *rhs)
{
    switch (operation)
    {
    case TT_PLUS:
        fprintf(code_output_file, "ADD LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_MINUS:
        fprintf(code_output_file, "SUB LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_ASTERISK:
        fprintf(code_output_file, "MUL LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_SLASH:
        fprintf(code_output_file, "IDIV LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_LESS:
        fprintf(code_output_file, "LT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_LESS_OR_EQUALS:
        fprintf(code_output_file, "GT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        fprintf(code_output_file, "NOT LF@%s LF@%s\n", result, result);
        break;
    case TT_EQUALS:
        fprintf(code_output_file, "EQ LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_NOT_EQUALS:
        fprintf(code_output_file, "EQ LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        fprintf(code_output_file, "NOT LF@%s LF@%s\n", result, result);
        break;
    case TT_GREATER:
        fprintf(code_output_file, "GT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_GREATER_OR_EQUALS:
        fprintf(code_output_file, "LT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        fprintf(code_output_file, "NOT LF@%s LF@%s\n", result, result);
        break;
    default:
        fprintf(stderr, "Unsupported operation!");
        return 1;
    }
    return 0;
}

int generate_arithmetic_operation_float(Token_type operation, char *result, char *lhs, char *rhs)
{
    switch (operation)
    {
    case TT_PLUS:
        fprintf(code_output_file, "ADD LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_MINUS:
        fprintf(code_output_file, "SUB LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_ASTERISK:
        fprintf(code_output_file, "MUL LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_SLASH:
        fprintf(code_output_file, "DIV LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_LESS:
        fprintf(code_output_file, "LT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_LESS_OR_EQUALS:
        fprintf(code_output_file, "GT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        fprintf(code_output_file, "NOT LF@%s LF@%s\n", result, result);
        break;
    case TT_EQUALS:
        fprintf(code_output_file, "EQ LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_NOT_EQUALS:
        fprintf(code_output_file, "EQ LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        fprintf(code_output_file, "NOT LF@%s LF@%s\n", result, result);
        break;
    case TT_GREATER:
        fprintf(code_output_file, "GT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        break;
    case TT_GREATER_OR_EQUALS:
        fprintf(code_output_file, "LT LF@%s LF@%s LF@%s\n", result, lhs, rhs);
        fprintf(code_output_file, "NOT LF@%s LF@%s\n", result, result);
        break;
    default:
        fprintf(stderr, "Unsupported operation!");
        return 1;
    }
    return 0;
}

int generate_if_head(char *compare_variable, int number)
{
    fprintf(code_output_file, "JUMPIFEQ $$IF_FALSE%d LF@%s bool@false\n", number, compare_variable);
    fprintf(code_output_file, "# True%d\n", number);
    return 0;
}

int generate_if_middle(int number)
{
    fprintf(code_output_file, "JUMP $$IF_END%d\n", number);
    fprintf(code_output_file, "LABEL $$IF_FALSE%d\n", number);
    fprintf(code_output_file, "# False%d\n", number);
    return 0;
}

int generate_if_end(int number)
{
    fprintf(code_output_file, "LABEL $$IF_END%d\n", number);
    fprintf(code_output_file, "# end%d\n", number);
    return 0;
}

int generate_for_head(char *check_operand_identifier, int for_counter)
{
    fprintf(code_output_file, "JUMPIFEQ $CYCLE%d LF@%s bool@true\n", for_counter, check_operand_identifier);
    fprintf(code_output_file, "JUMP $END%d\n", for_counter);
    return 0;
}

int generate_for_label_end(int for_counter)
{
    fprintf(code_output_file, "LABEL $END%d\n", for_counter);
    return 0;
}

int generate_for_label_cycle(int for_counter)
{
    fprintf(code_output_file, "LABEL $CYCLE%d\n", for_counter);
    return 0;
}

int generate_for_iterate(char *check_operand_identifier, int for_counter)
{
    fprintf(code_output_file, "JUMPIFEQ $CYCLE%d LF@%s bool@true\n", for_counter, check_operand_identifier);
    fprintf(code_output_file, "JUMP $END%d\n", for_counter);
    return 0;
}