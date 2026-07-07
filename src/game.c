#include <stdbool.h>
#include "stack.h"
#include "deck.h"

// steps to game
// dealer can be the computer
// for 2 players 7 cards dealt to each player




// End condition: all thirteen books have been won
// Books can belong to a player
// Players: 1 human 1 bot (for now)

// Define a player
    // has an id
    // has a hand
    // has won books
// Define a book
    // has cards
    // is completed
    // owner?

stack_card drawPile; //hopeful this is okay global

void deckToStack(card deck[]) {
    stackInit(&drawPile);

    for (int i = 0; i < DECK_SIZE; i++) {
        stackPush(&drawPile, deck[i]);
    }
}

void gameInit() {
    shuffleDeck(deck);
    deckToStack(deck);
}

void game() {
    gameInit();


}
