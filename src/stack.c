#include "stack.h"
#include <stdbool.h>
#include <stdio.h>

void stackInit(StackCard *stack) { stack->size = 0; }

bool stackPush(StackCard *stack, Card value) {
    if (stack->size >= DECK_SIZE) {
        fprintf(stderr, "Error: stack limit exceeded");
        return false;
    }

    stack->cards[stack->size] = value;
    stack->size++;

    return true;
}

bool stackPop(StackCard *stack, Card *drawn) {
    if (stack->size <= 0) {
        fprintf(stderr, "Error: stack is empty");
        return false;
    }

    stack->size--;
    *drawn = stack->cards[stack->size];

    return true;
}

bool stackIsEmpty(StackCard *stack) {
    if (stack->size == 0) {
        return true;
    } else {
        return false;
    }
}
