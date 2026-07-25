#ifndef STACK_H
#define STACK_H

#include "deck.h"
#include <stdbool.h>

typedef struct {
    card cards[52];
    int size;
} stack_card;

void stackInit(stack_card *stack);

bool stackPush(stack_card *stack, card value);

bool stackPop(stack_card *stack, card *drawn);

bool stackIsEmpty(stack_card *stack);

#endif