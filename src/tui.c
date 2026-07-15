#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "deck.h"
#include "game.h"

//PUBLIC FACING FUNCTIONS PREFIXED WITH tui_
//i.e. tui_doSomething()

void tui_clearScreen() {
    printf("\e[1;1H\e[2J"); //black magic
    fflush(stdout);
}

void tui_newline(int howMuch) {
    for (int i = 0; i < howMuch; i++) {
        printf("\n");
    }
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

static void printWonBookValues(game_state *game, int playerId) {
    for (int i = 0; i < game->sizeOfBooks; i++) {
        if (game->books[i].ownerId == playerId) {
            printf(" (%d) ", game->books[i].bookValue);
        }
    }
    printf("\n");
}

static void printCardsInHand(player *player, bool hidden) {
    for(int i = 0; i < player->handSize; i++) {
        if(hidden){
            printf(" ## ");
        } else {
            char cardShorthand[4];
            cardToShorthand(player->hand[i], cardShorthand, sizeof(cardShorthand));
            printf(" %s ", cardShorthand);
        }

    }
}

static void displayBooks(game_state *game) {

    printf("Game ends when all 13 books are won\n");
    printf("Number of books won: %d\n", game->sizeOfBooks);

    for(int i = 0; i < game->playerCount; i++) {
        printf("Player %d's books: ", game->players[i].playerNum + 1);
        printWonBookValues(game, i);
    }
}

static void displayHands(game_state *game) {
    //this is just the current way of handling this.
    //TODO: will need to make this scale with multiple players

    //Find the user and print the opponents hand first
    int usersId;
    for(int i = 0; i < game->playerCount; i++) {
        if(game->players[i].isUser) {
            usersId = game->players[i].playerNum;
        } else {
            printf("Opponent's Hand:");
            printCardsInHand(&game->players[i], false);
        }
    }

    tui_newline(3);

    printf("Cards in Stack: %d", game->drawPile.size);

    tui_newline(3);

    printf("Your Hand:");
    printCardsInHand(&game->players[usersId], false);
    tui_newline(1);
}

void tui_displayTurn(game_state *game) {
    tui_clearScreen();
    displayBooks(game);
    tui_newline(3);
    displayHands(game);
}

void tui_displayWin(game_state *game) {
    tui_clearScreen();
    printf("Game Won");
}

static bool isValidCardInput(const char *cardInput) {
    size_t length = strlen(cardInput);

    //size validation
    if (!(length == 2 || length == 3)) {
        return false;
    }

    //since suit will always be one character
    char inputSuit = cardInput[length - 1];

    //suit validation
    if (!(inputSuit == 'H' || inputSuit == 'D' || inputSuit == 'C' || inputSuit == 'S')) {
        return false;
    }

    //if not a 10, else a 10
    if (length == 2) {
        char inputValue = cardInput[0];

        if (
            (inputValue >= '2' && inputValue <= '9') 
            || inputValue == 'J'
            || inputValue == 'Q'
            || inputValue == 'K'
            || inputValue == 'A'
        ) {
            return true;
        }
    } else {
        if(cardInput[0] == '1' && cardInput[1] == '0') {
            return true;
        }
    }

    return false;
}

void tui_askForCard(char *playerInput, size_t playerInputSize) {
    while(true) {
        printf("Ask for card: ");

        //handles basic fgets validation, and returns empty string if something goes wrong
        if(fgets(playerInput, playerInputSize, stdin) == NULL) {
            playerInput[0] = '\0';
            return;
        }

        //removes newline from input
        char *newline = strchr(playerInput, '\n');
        if (newline != NULL) {
            *newline = '\0';
        } else {
            //clears for next fgets()
            int character;
            while ((character = getchar()) != '\n' && character != EOF); //pretty dumb, i think this removes old inputs from fgets
            printf("Invalid request... Please ask for a valid card (AC, 10H, 9D, JS etc)\n");
            continue;
        }

        //force input to uppercase
        for (size_t i = 0; playerInput[i] != '\0'; i++) {
            playerInput[i] = (char)toupper((unsigned char)playerInput[i]);
        }

        if(isValidCardInput(playerInput)) {
            return;
        } else {
            printf("Invalid request... Please ask for a valid card (AC, 10H, 9D, JS, etc)\n");
        }
    }
}

//game.c does functions
//tui.c does visuals