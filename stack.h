/*------------------------------------------
 Title:  parser.c
 Author: Koval Maksym
 Login:  xkoval20
 Date:   23 Nov 2021;
------------------------------------------*/

#define EMPTY 999
#pragma once
typedef struct tokenStack Stack;

typedef struct tokenStack
{
    Token head;  //root
    Stack *next; //next token
} tokenStack;

/**
 * Free stack
 * @param stack stack to free
 **/
void stackFree(Stack *stack);

/**
*   Init stack, use a stack to init
*   @param stack init Stack
**/
void stackInit(Stack *stack);

/**
*   Returning top of stack or NULL if empty
*   @param stack Stack user using
*   @return top of stack
**/
Token stackTop(Stack *stack);

/**
 * Cleaning stack Tokens
 * @param stack cleaned stack
 **/
void stackClear(Stack *stack);

/**
*   Pop first token from stack or return if empty
*   @param stack Stack where pop
*   @return Return if is nothing to pop
**/
void stackPop(Stack *stack);

/**
*   Push on top of stack
*   @param stack Stack where push
*   @param new token pushing in stack
**/
void stackPush(Stack *stack, Token new);

/**
*   Reversing stack 
*   @param stack Stack top reverse
**/
Stack stackReverse(Stack *stack);