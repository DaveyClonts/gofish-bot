#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "game.h"

int main(void) {
    srand(time(NULL)); //random gen for shuffle in deck.c

    gameInit();

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