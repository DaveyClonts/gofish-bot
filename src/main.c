#include <stdlib.h>
#include <time.h>
#include "deck.h"

int main(void) {
    srand(time(NULL)); //random gen for shuffle in deck.c

    shuffleDeck(deck);
    printDeck(deck);
    return 0;
}