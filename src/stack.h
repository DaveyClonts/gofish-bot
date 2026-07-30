#ifndef STACK_H
#define STACK_H

#include "deck.h"
#include <stdbool.h>

typedef struct {
    Card cards[52];
    int size;
} StackCard;

void stackInit(StackCard *stack);

bool stackPush(StackCard *stack, Card value);

bool stackPop(StackCard *stack, Card *drawn);

bool stackIsEmpty(StackCard *stack);

#endif
