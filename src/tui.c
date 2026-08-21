#include "tui.h"
#include "deck.h"
#include "event_stream.h"
#include "game.h"
#include "player.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// PUBLIC FACING FUNCTIONS PREFIXED WITH tui_
// i.e. tui_doSomething()

void tui_clearScreen() {
    printf("\x1b[1;1H\x1b[2J"); // black magic
    fflush(stdout);
}

void tui_newline(int howMuch) {
    for (int i = 0; i < howMuch; i++) {
        printf("\n");
    }
}

void tui_startScreen() {
    puts(" ,----.                 ,------.,--.       ,--. \n"
         "'  .-./    ,---. ,-----.|  .---'`--' ,---. |  ,---.\n"
         "|  | .---.| .-. |'-----'|  `--, ,--.(  .-' |  .-.  | \n"
         "'  '--'  |' '-' '       |  |`   |  |.-'  `)|  | |  | \n"
         " `------'  `---'        `--'    `--'`----' `--' `--' \n");

    printf("\n\n\nReady to Play?\n");
    printf("Press any key to start...");

    getchar();
}

void tui_winScreen(GameState *game) {
    printf("\nGame Won!\n");
    printf("\nWinner: player %d", game->winCondition.winnerId);
}

static void printWonBookValues(GameState *game, int playerId) {
    for (int i = 0; i < game->sizeOfBooks; i++) {
        if (game->books[i].ownerId == playerId) {
            printf(" (%s) ", valueToShorthand(game->books[i].bookValue));
        }
    }
    printf("\n");
}

static void printCardsInHand(Player *player, bool hidden) {
    for (int i = 0; i < player->handSize; i++) {
        if (hidden) {
            printf(" ## ");
        } else {
            char cardShorthand[4];
            cardToShorthand(player->hand[i], cardShorthand, sizeof(cardShorthand));
            printf(" %s ", cardShorthand);
        }
    }
}

static void displayBooks(GameState *game) {

    printf("Game ends when all 13 books are won\n");
    printf("Number of books won: %d\n", game->sizeOfBooks);

    for (int i = 0; i < game->playerCount; i++) {
        printf("Player %d's books: ", game->players[i].playerNum + 1);
        printWonBookValues(game, i);
    }
}

static void displayHands(GameState *game) {
    // this is just the current way of handling this.
    // TODO: will need to make this scale with multiple players

    // Find the user and print the opponents hand first
    int usersId;
    for (int i = 0; i < game->playerCount; i++) {
        if (game->players[i].isUser) {
            usersId = game->players[i].playerNum;
        } else {
            printf("Opponent's Hand:");
            printCardsInHand(&game->players[i], true);
        }
    }

    tui_newline(3);

    printf("Cards in Stack: %d", game->drawPile.size);

    tui_newline(3);

    printf("Your Hand:");
    printCardsInHand(&game->players[usersId], false);
    tui_newline(1);
}

static void displayLastEvent(GameState *game) {
    EventLog log = game->stream.eventLog;

    // realized my solution is crappy but i dont want to refactor
    // proper solution is to have each event hold a string
    //*facepalm*
    for (int i = (int)log.length; i > 0; i--) {
        if (strstr(log.log[i - 1], "turn") != NULL) {
            for (size_t j = i - 1; j < log.length; j++) {
                printf("%s", log.log[j]);
            }
            break;
        }
    }
}

void tui_waitForKey() {
    printf("\nPress Enter to continue...");
    fflush(stdout); // flush clears in the buffer so it can take input

    int character;
    while ((character = getchar()) != '\n' && character != EOF)
        ;
}

void tui_displayTurn(GameState *game) {
    tui_clearScreen();
    displayBooks(game);
    tui_newline(3);
    for (int i = 0; i < game->playerCount; i++) {
        player_organizeHand(&game->players[i]);
    }
    displayHands(game);
    displayLastEvent(game);
}

static bool checkForValidInput(Player *player, const char *buffer) {
    bool validInput = false;

    if (player->handSize == 0) {
        return true;
    }

    for (int i = 0; i < player->handSize; i++) {
        if (strcmp(buffer, valueToShorthand(player->hand[i].value)) == 0) {
            validInput = true;
            return validInput;
        }
    }

    return validInput;
}

static bool isValidValue(Player *player, const char *buffer) {
    size_t length = strlen(buffer);

    if (!checkForValidInput(player, buffer)) {
        return false;
    }

    if (length == 2) {
        return (buffer[0] == '1' && buffer[1] == '0');
    }

    if (length == 1) {
        return (buffer[0] >= '2' && buffer[0] <= '9' || buffer[0] == 'J' || buffer[0] == 'Q' ||
                buffer[0] == 'K' || buffer[0] == 'A');
    }

    return false;
}

bool tui_askForCard(Player *player, char *buffer, int bufferSize) {
    while (true) {
        printf("Request a card: ");

        if (fgets(buffer, bufferSize, stdin) == NULL) {
            buffer[0] = '\0'; // returns empty string if empty input
            return false;
        }

        char *newline = strchr(buffer, '\n');
        if (newline != NULL) {
            *newline = '\0'; // replaces newline with a string terminator
        } else {
            // cleans out stdin since NULL means input was too long
            int character;
            while ((character = getchar()) != '\n' && character != EOF)
                ;
            printf("Input too long...\n");
            continue;
        }

        // force input to uppercase
        for (size_t i = 0; buffer[i] != '\0'; i++) {
            buffer[i] = (char)toupper((unsigned char)buffer[i]);
        }

        if (isValidValue(player, buffer)) {
            return true;
        } else {
            printf("Invalid request...\n");
        }
    }
}
