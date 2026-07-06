// Public Headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Headers
#include "deck.h"

int main(void) {
    srand(time(NULL)); //random gen for shuffle in deck.c

    shuffleDeck(deck);
    printDeck(deck);
}