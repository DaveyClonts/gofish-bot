#include "game.h"
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand(time(NULL)); // random gen for shuffle in deck.c

    gameInit();

    startGame();

    return 0;
}