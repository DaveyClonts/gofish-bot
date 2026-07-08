#ifndef DECK_H
#define DECK_H
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

void readCard(card card);

void printDeck(card deck[]);

void shuffleDeck(card deck[]);

#endif