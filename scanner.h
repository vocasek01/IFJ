/*------------------------------------------
 Title:  scanner.h
 Author: Matěj Krátký, Golikov Ivan
 Login:  xkratk17, xgolik00 
 Date:   15 Oct 2021;
------------------------------------------*/


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "error.h"

#define MAX_LENGTH 1024
#pragma once

typedef enum {

    //Expression type
    E_NONTERM_INT,
    E_NONTERM_FLOAT,
    E_NONTERM_STR,
    E_NONTERM_BOOL,
    E_NONTERM_NIL,
    E_NONTERM_ZERO_INT,
    E_NONTERM_RULE,
    E_NONTERM_ID,
    E_STOP,
    E_SHIFT,

    START = 100,

    //Keyword or Identificator
    IDENTIFICATOR,   // text bez (if,else,for,return,package,func)
    KEYWORD,         // if, else, for, return, package, func
    IDENORKEY,        //

    //Type of date
    INT,      // celé číslo 42

    DOUBLE,    // 4.2, nebo 4e+447
    DOUB_DOT1, // 
    DOUB_DOT2, //
    DOUB_EXP1, //
    DOUB_EXP2,


    STR,      // "text"
    ESCAPE1,  //
    ESCAPE2,  //

    IFJ21,    // "ifj21"


    COLON,    // :
    COMMA,    // ,

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

    //Lex. error or Expr error
    ERROR =-1,

} TokenType;

typedef struct {
    char * attribute;
    TokenType type;
} Token;

/**
*   Init scanner, use a file from which to read
*   @param filename Name of file
*   @return true or false
**/
bool scannerInit(FILE* file);

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
