#include <stdio.h>
#include "game.h"

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

void displayWonBookValues(game_state *game, int playerId) {
    for (int i = 0; i < game->sizeOfBooks; i++) {
        if (game->books[i].ownerId == playerId) {
            printf(" (%d) ", game->books[i].bookValue);
        }
    }
    printf("\n");
}

void tui_displayHands(game_state *game) {

    tui_clearScreen();

    printf("Game ends when all 13 books are won\n");
    printf("Number of books won: %d\n", game->sizeOfBooks);

    for(int i = 0; i < game->playerCount; i++) {
        printf("Player %d's books: ", game->players[i].playerNum + 1);
        displayWonBookValues(game, i);
    }

    

    
}