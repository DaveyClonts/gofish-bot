#ifndef BOT_H
#define BOT_H
#include "deck.h"

// Certain: A player just asked for Queens, so they currently have a Queen.
// Strong: A player received two Queens and has not completed the book.
// Weak: A player said “Go Fish” several turns ago, but may have drawn a Queen since then.
typedef enum {
    CERTAIN,
    STRONG,
    WEAK
} certaintiy;

typedef struct {
    card card;
    certaintiy certaintiy;
    int targetId;
} memoryCard;

typedef struct {
    memoryCard *remeberedCards;
    int size;
    int capacity;
} memory;

#endif