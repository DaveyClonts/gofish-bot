#include <stdio.h>

void clearScreen() {
    printf("\e[1;1H\e[2J"); //black magic
    fflush(stdout);
}

void startScreen() {
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