#include "deck.h"
#include <stdbool.h>


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

struct book {
    card cardsInBook[4];
    bool completed;
    int ownerID;
};

struct player {
    int playerNum;

};