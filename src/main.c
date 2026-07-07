#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "game.h"

void clearScreen() {
    printf("\e[1;1H\e[2J"); //black magic
    fflush(stdout);
}

int main(void) {
    srand(time(NULL)); //random gen for shuffle in deck.c

    clearScreen();

    puts(
        " ,----.                 ,------.,--.       ,--. \n"
        "'  .-./    ,---. ,-----.|  .---'`--' ,---. |  ,---.\n"
        "|  | .---.| .-. |'-----'|  `--, ,--.(  .-' |  .-.  | \n"
        "'  '--'  |' '-' '       |  |`   |  |.-'  `)|  | |  | \n"
        " `------'  `---'        `--'    `--'`----' `--' `--' \n"
    );

    printf("\n\n\nReady to Play?\n");
    printf("Press any key to start...");

    getchar();

    return 0;
}