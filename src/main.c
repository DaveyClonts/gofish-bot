#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "tui.h"


int main(void) {
    srand(time(NULL)); //random gen for shuffle in deck.c

    tui_clearScreen();
    tui_startScreen();

    gameInit();

    startGame();

    return 0;
}