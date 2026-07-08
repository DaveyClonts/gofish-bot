#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "tui.h"


int main(void) {
    srand(time(NULL)); //random gen for shuffle in deck.c

    clearScreen();
    startScreen();

    gameInit();

    runGame();

    return 0;
}