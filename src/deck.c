#include "deck.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *suit_names[] = {"Clubs", "Diamonds", "Hearts", "Spades"};

const char *suit_shorthand[] = {"C", "D", "H", "S"};

const char *value_names[] = {"Two",  "Three", "Four", "Five",  "Six",  "Seven", "Eight",
                             "Nine", "Ten",   "Jack", "Queen", "King", "Ace"};

const char *value_shorthand[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

Card g_deck[52] = {
    // CLUB
    {CLUB, TWO},
    {CLUB, THREE},
    {CLUB, FOUR},
    {CLUB, FIVE},
    {CLUB, SIX},
    {CLUB, SEVEN},
    {CLUB, EIGHT},
    {CLUB, NINE},
    {CLUB, TEN},
    {CLUB, JACK},
    {CLUB, QUEEN},
    {CLUB, KING},
    {CLUB, ACE},

    // DIAMOND
    {DIAMOND, TWO},
    {DIAMOND, THREE},
    {DIAMOND, FOUR},
    {DIAMOND, FIVE},
    {DIAMOND, SIX},
    {DIAMOND, SEVEN},
    {DIAMOND, EIGHT},
    {DIAMOND, NINE},
    {DIAMOND, TEN},
    {DIAMOND, JACK},
    {DIAMOND, QUEEN},
    {DIAMOND, KING},
    {DIAMOND, ACE},

    // HEART
    {HEART, TWO},
    {HEART, THREE},
    {HEART, FOUR},
    {HEART, FIVE},
    {HEART, SIX},
    {HEART, SEVEN},
    {HEART, EIGHT},
    {HEART, NINE},
    {HEART, TEN},
    {HEART, JACK},
    {HEART, QUEEN},
    {HEART, KING},
    {HEART, ACE},

    // SPADES
    {SPADES, TWO},
    {SPADES, THREE},
    {SPADES, FOUR},
    {SPADES, FIVE},
    {SPADES, SIX},
    {SPADES, SEVEN},
    {SPADES, EIGHT},
    {SPADES, NINE},
    {SPADES, TEN},
    {SPADES, JACK},
    {SPADES, QUEEN},
    {SPADES, KING},
    {SPADES, ACE}};

Card shorthandToCard(char *shorthand) {
    Card newCard;

    size_t length = strlen(shorthand);
    char value[3];
    char suit[2];

    suit[0] = shorthand[length - 1];
    suit[1] = '\0';

    if (length == 2) {
        value[0] = shorthand[0];
        value[1] = '\0';
    } else {
        value[0] = '1';
        value[1] = '0';
        value[2] = '\0';
    }

    for (int i = TWO; i <= ACE; i++) {
        if (strcmp(value, value_shorthand[i]) == 0) {
            newCard.value = i;
            break;
        }
    }

    for (int i = CLUB; i <= SPADES; i++) {
        if (strcmp(suit, suit_shorthand[i]) == 0) {
            newCard.suit = i;
            break;
        }
    }

    return newCard;
}

values shorthandToValues(char *shorthand) {
    values value;

    for (int i = 0; i <= 12; i++) {
        if (strcmp(shorthand, value_shorthand[i]) == 0) {
            value = i;
        }
    }

    return value;
}

const char *valueToShorthand(values value) { return value_shorthand[value]; }

const char *suitToShorthand(suits suit) { return suit_shorthand[suit]; }

void cardToShorthand(Card card, char output[], size_t size) {
    snprintf(output, size, "%s%s", value_shorthand[card.value], suit_shorthand[card.suit]);
}

void readCard(Card card) { printf("%s of %s \n", value_names[card.value], suit_names[card.suit]); }

void printDeck(Card deck[]) {
    for (int i = 0; i < DECK_SIZE; i++) {
        readCard(deck[i]);
    }
}

// Fisher-Yates shuffle algo
void shuffleDeck(Card deck[]) {
    for (int i = 0; i < DECK_SIZE; i++) {
        int toBeSwapped = i + rand() % (DECK_SIZE - i);

        Card temp = deck[i];
        deck[i] = deck[toBeSwapped];
        deck[toBeSwapped] = temp;
    }
}
