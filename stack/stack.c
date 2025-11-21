#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "common.h"

void stackInit(struct Stack *stack)
{
    stack->top = 0;
}

int stackPush(struct Stack *stack, char *string)
{
    if (stack->top >= MAX_ITEMS)
        return -1;
    if (strlen(string) > MAX_LEN)
        return -1;
    
    strncpy(stack->items[stack->top], string, strlen(string));
    stack->top++;
    return 0;
}

char* stackPop(struct Stack *stack)
{
    if (stack->top == 0)
        return NULL;
    
    stack->top--;
    return stack->items[stack->top];
}
