#ifndef DECK_H
#define DECK_H
#include <stddef.h>
#define DECK_SIZE 52

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
} values;

typedef struct {
    suits suit;
    values value;
} card;

extern card g_deck[52];

card shorthandToCard(char *shorthand);

values shorthandToValues(char *shorthand);

const char *valueToShorthand(values value);

const char *suitToShorthand(suits suit);

void cardToShorthand(card card, char output[], size_t size);

void readCard(card card);

void printDeck(card deck[]);

void shuffleDeck(card deck[]);

#endif