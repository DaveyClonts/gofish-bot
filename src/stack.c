#include "stack.h"
#include <stdio.h>
#include <stdbool.h>

void stackInit(stack_card *stack) {
    stack->size = 0;
}

bool stackPush(stack_card *stack, card value) {
    if (stack->size >= 52) {

        fprintf(stderr, "Error: stack limit exceeded");
        return false;
    }

    stack->cards[stack->size] = value;
    stack->size++;

    return true;
}

bool stackPop(stack_card *stack, card *drawn) {
    if(stack->size <= 0) {
        fprintf(stderr, "Error: stack is empty");
        return false;
    }

    stack->size--;
    *drawn = stack->cards[stack->size];

    return true;
}

bool stackPeek(stack_card *stack, card *peeked) {
    if(stack->size <= 0) {
        fprintf(stderr, "Error: stack is empty");
        return false;
    }

    *peeked = stack->cards[stack->size];
    
    return true;
}