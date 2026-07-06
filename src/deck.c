#include "deck.h"
#include <stdio.h>
#include <stdlib.h>

const char *suit_names[] = {
    "Clubs",
    "Diamonds",
    "Hearts",
    "Spades"
};

const char *value_names[] = {
    "Two",
    "Three",
    "Four",
    "Five",
    "Six",
    "Seven",
    "Eight",
    "Nine",
    "Ten",
    "Jack",
    "Queen",
    "King",
    "Ace"
};

card deck[52] = {
    // CLUB
    {CLUB, TWO}, {CLUB, THREE}, {CLUB, FOUR}, {CLUB, FIVE}, {CLUB, SIX},
    {CLUB, SEVEN}, {CLUB, EIGHT}, {CLUB, NINE}, {CLUB, TEN}, {CLUB, JACK},
    {CLUB, QUEEN}, {CLUB, KING}, {CLUB, ACE},

    // DIAMOND
    {DIAMOND, TWO}, {DIAMOND, THREE}, {DIAMOND, FOUR}, {DIAMOND, FIVE}, {DIAMOND, SIX},
    {DIAMOND, SEVEN}, {DIAMOND, EIGHT}, {DIAMOND, NINE}, {DIAMOND, TEN}, {DIAMOND, JACK},
    {DIAMOND, QUEEN}, {DIAMOND, KING}, {DIAMOND, ACE},

    // HEART
    {HEART, TWO}, {HEART, THREE}, {HEART, FOUR}, {HEART, FIVE}, {HEART, SIX},
    {HEART, SEVEN}, {HEART, EIGHT}, {HEART, NINE}, {HEART, TEN}, {HEART, JACK},
    {HEART, QUEEN}, {HEART, KING}, {HEART, ACE},

    // SPADES
    {SPADES, TWO}, {SPADES, THREE}, {SPADES, FOUR}, {SPADES, FIVE}, {SPADES, SIX},
    {SPADES, SEVEN}, {SPADES, EIGHT}, {SPADES, NINE}, {SPADES, TEN}, {SPADES, JACK},
    {SPADES, QUEEN}, {SPADES, KING}, {SPADES, ACE}
};
//better way to do this might be to make an ID that is H2-HA or what not... idk ill come back to this

void readCard(card card) {
    printf("%s of %s \n", value_names[card.value], suit_names[card.suit]);
}

void printDeck(card deck[]) {
    for (int i = 0; i < DECK_SIZE; i++){
        readCard(deck[i]);
    }
}

//Fisher-Yates shuffle algo
void shuffleDeck(card deck[]) {
    for (int i = 0; i < DECK_SIZE; i++) {
        int toBeSwapped = i + rand() % (DECK_SIZE - i);

        card temp = deck[i];
        deck[i] = deck[toBeSwapped];
        deck[toBeSwapped] = temp;
    }
}

