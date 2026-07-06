#ifndef DECK_H
#define DECK_H

typedef enum {
    CLUB,
    DIAMOND,
    HEART,
    SPADES
} suits;

typedef enum {
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE
} types;

typedef struct {
    suits suit;
    types type;
} card;

extern const card deck[52];

#endif