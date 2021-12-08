/*------------------------------------------
 Title:  stack.c
 Author: Koval Maksym
 Login:  xkoval20
 Date:   23 Nov 2021;
------------------------------------------*/

#include <stdlib.h>
#include "stack.h"

void stackInit(Stack *stack)
{
    Token tmp;
    tmp.type = EMPTY;
    tmp.attribute = NULL;

    stack->head = tmp;
    
    stack->next = NULL;
}



Token stackTop(Stack *stack)
{
    return stack->head;
}


void stackPop(Stack *stack)
{
    Stack *tmp;

    if (stack == NULL)
    {
        return;
    }
    else if (stack->head.type == EMPTY)
    {
        return;
    }

    tmp = stack->next;
    stack->head = stack->next->head;
    stack->next = stack->next->next;
    free(tmp);
}


void stackPush(Stack *stack, Token new)
{
    Stack *tmp = (Stack *)malloc(sizeof(Stack));
    tmp->head = stack->head;
    tmp->next = stack->next;
    stack->head = new;
    stack->next = tmp;
}



void stackClear(Stack *stack)
{
    Stack *result = stack;
    while (result->head.type != EMPTY)
    {
        stackPop(stack);
        result = stack;
    }
}

Stack stackReverse(Stack *stack)
{
    Stack clone;
    stackInit(&clone);

    Stack *result = stack;
    while (result->head.type != EMPTY)
    {
        Token x = stackTop(stack);
        stackPush(&clone,x);
        stackPop(stack);
        result = stack;
    }

    stack = &clone;
    return *stack;
}

void stackFree(Stack *stack)
{
    if (stack == NULL || stack->head.type == EMPTY)
    {
        return;
    }
    Stack *tmp = stack;
    while (tmp != NULL)
    {
        tmp = stack->next;
        free(stack);
        stack = tmp;
    }
}
