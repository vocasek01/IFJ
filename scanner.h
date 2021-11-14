#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 1024
#pragma once

typedef enum {
    START,

    //Keyword or Identificator
    IDENTIFICATOR,   // text bez (if,else,for,return,package,func)
    KEYWORD,         // if, else, for, return, package, func
    IDENORKEY,        //

    //Type of date
    INT,      // celé číslo 42

    DOUBLE,    // 4.2, nebo 4e+4.47
    DOUB_DOT1, // 
    DOUB_DOT2, //
    DOUB_EXP1, //
    DOUB_EXP2,


    STR,      // "text"
    ESCAPE1,  //
    ESCAPE2,  //

    //Opetators
    ADD,       // +
    SUB,       // -
    MUL,       // *
    DIV,       // /
    INT_DIV,   // //
    LEN,       // #
    GT,        // >
    LT,        // <
    NOTEQ,     // !=
    GTE,       // >=
    LTE,       // <=
    EQ,        // ==
    KONC,      // ..

    ASSIGN,    // =
    RBR,       // )
    LBR,       // (

    //Other
    ENDOFFILE,    //EOF
    EOL,          //EOL
    COMMENT,      // --text
    BLOCKCOMMENT, // --[ text ]
    BLOCKORLINE,  // which comment

    //Lex. error
    ERROR =-1,

} State;

typedef struct {
    char * attribute;
    State type;
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
 *  Add a string to the token attributes
 **/

void getString(char *str, Token *token);

/**
*   Read text letter by letter and comparing with specified conditions
*   @return Token token with specific name and type
**/
Token getToken();


/**
*   Check key word
**/
bool isKeyWord(char *str);