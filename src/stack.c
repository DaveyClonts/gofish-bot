#include "stack.h"
#include <stdbool.h>
#include <stdio.h>

void stackInit(stack_card *stack) { stack->size = 0; }

bool stackPush(stack_card *stack, card value) {
    if (stack->size >= DECK_SIZE) {
        fprintf(stderr, "Error: stack limit exceeded");
        return false;
    }

    stack->cards[stack->size] = value;
    stack->size++;

    return true;
}

bool stackPop(stack_card *stack, card *drawn) {
    if (stack->size <= 0) {
        fprintf(stderr, "Error: stack is empty");
        return false;
    }

    stack->size--;
    *drawn = stack->cards[stack->size];

    return true;
}

bool stackIsEmpty(stack_card *stack) {
    if (stack->size == 0) {
        return true;
    } else {
        return false;
    }
}
