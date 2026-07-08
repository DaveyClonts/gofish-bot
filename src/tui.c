#include <stdio.h>

void tui_clearScreen() {
    printf("\e[1;1H\e[2J"); //black magic
    fflush(stdout);
}

void tui_startScreen() {
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
}

void tui_displayHands() {
    printf("\n\n\n\n\n");

    printf("Opponent's Hand: ");

    
}