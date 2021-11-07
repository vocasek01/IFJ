#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*
 *****************************************************
 * 
 *                   define                  
 * 
*/
#define MAX_LENGTH 1024
#pragma once 
typedef enum
{
    STATE_START,
    STATE_ID,
    STATE_ID_FN,
    STATE_SPACE,
    STATE_QUOTATION_MARK,
    STATE_NUMBER,
    STATE_EXPR,
    STATE_COMMENT,
    STATE_STR4,
    STATE_STR3,
    STATE_STR2,
    STATE_DOUBLE,
    STATE_DOUBLE2,
    STATE_EXP,
    STATE_EXP2,
    STATE_EXP3,
    STATE_NOTEQ,
    STATE_DOUBLEDOT,
    STATE_EQL,
    STATE_AND,
    STATE_OR,
    STATE_DIV,
    STATE_LTE,
    STATE_GTE
} State;

typedef enum
{
    ID,        //text bez (if,else,for,return,package,func)
    INT,       //celé číslo 42
    DOUBLE,    // 4.2, nebo 4e+4.47
    ENDOFFILE, //EOF
    EOL,       //EOL
    SPACE,     // ' '
    EXPR,      // &&, ||
    RCBR,      // }
    LCBR,      // {
    RBR,       // )
    LBR,       // (
    COMA,      // ,
    DOT,       // .
    ADD,       // +
    SUB,       // -
    MUL,       // *
    DIV,       // /
    GT,        // >
    LT,        // <
    GTE,       // >=
    LTE,       // <=
    COMMENT,   // //text
    SEMICOLON, // ;
    STRING,    // "text"
    IF,
    ELSE,
    FOR,
    RETURN,
    PACKAGE,
    FUNC,
    STRING_TYPE,
    INT_TYPE,
    FLOAT64_TYPE,
    BOOLEAN_TYPE,
    FUNC_INPUTS,
    FUNC_INPUTI,
    FUNC_INPUTF,
    FUNC_PRINT,
    FUNC_INT2FLOAT,
    FUNC_FLOAT2INT,
    FUNC_LEN,
    FUNC_SUBSTR,
    FUNC_ORD,
    FUNC_CHR,
    ASSIGN, // =
    DEF,    // :=
    NOTEQ,  // !=
    EQL,    // ==
    ERROR,
    NONTERM = 502,
} tokenType;

typedef union
{
    int integer_num;
    float float_num;
    char *string;
} tokenAttribute;

typedef struct
{
    tokenType type;
    tokenAttribute atrribute;
    int count_param;
    int return_param;
} tData;

typedef struct tBSTNode
{
    char *ID;
    bool isFunction;
    tData BSTNodeCont;
    struct tBSTNode *LPtr;
    struct tBSTNode *RPtr;
} * tBSTNodePtr;

typedef struct
{
    tokenAttribute attribute;
    char *tokenName;
    tokenType type;
    tBSTNodePtr symTable;
    int error_flag;
} Token;

/**
*   Init scanner, use a file from which to read
*   @param filename Name of file
*   @return true or false
**/
bool scannerInit(char* file);

/**
*   Write errorCode in stderr
**/
void errorCode();

/**
*   Read text letter by letter and comparing with specified conditions
*   @return Token token with specific name and type
**/
Token getToken();