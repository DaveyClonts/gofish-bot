#include "game.h"
#include "deck.h"
#include "stack.h"
#include "tui.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int G_STARTING_HAND_SIZE = 7;

game_state g_game;

static void deckToStack(game_state *game, card deck[]) {
    stackInit(&game->drawPile);

    for (int i = 0; i < DECK_SIZE; i++) {
        stackPush(&game->drawPile, deck[i]);
    }
}

static void playersInit(game_state *game) {
    for (int i = 0; i < game->playerCount; i++) {
        game->players[i].playerNum = i;
        game->players[i].handSize = 0;
        game->players[i].capacity = G_STARTING_HAND_SIZE;
        game->players[i].hand = malloc(game->players[i].capacity * sizeof(card));

        if (game->players[i].hand == NULL) {
            fprintf(stderr, "Error: memory allocation failed");
            exit(EXIT_FAILURE);
        }
    }
    game->players[0].isUser = true; // forced for now
}

static void checkHandCapacity(player *player) {
    if (player->handSize >= player->capacity) {
        player->capacity *= 2;
        card *reallocHand = realloc(player->hand, player->capacity * sizeof(card));

        if (reallocHand == NULL) {
            fprintf(stderr, "Error: hand realloc failed");
            exit(EXIT_FAILURE);
        }

        player->hand = reallocHand;
    }
}

static void drawCard(game_state *game, player *player) {

    checkHandCapacity(player);

    if (stackPop(&game->drawPile, &player->hand[player->handSize])) {
        player->handSize++;
    }
}

static void giveCardToPlayer(player *giver, player *reciever, int cardIndex) {
    if (giver->handSize <= 0) {
        fprintf(stderr, "Error: giver's hand is empty");
        exit(EXIT_FAILURE);
    }

    if (cardIndex < 0 || cardIndex >= giver->handSize) {
        fprintf(stderr, "Error: cardIndex is out of bounds");
        exit(EXIT_FAILURE);
    }

    // shifts cards left and reduces handSize
    card passingCard = giver->hand[cardIndex];
    for (int i = cardIndex; i < giver->handSize - 1; i++) {
        giver->hand[i] = giver->hand[i + 1];
    }
    giver->handSize--;

    checkHandCapacity(reciever);
    reciever->hand[reciever->handSize] = passingCard;
    reciever->handSize++;
}

static void giveCardToBook(game_state *game, book *book, int giverId, int cardIndex) {
    player *giver = &game->players[giverId];

    card passingCard = giver->hand[cardIndex];

    for (int i = cardIndex; i < giver->handSize - 1; i++) {
        giver->hand[i] = giver->hand[i + 1];
    }

    giver->handSize--;
    book->cardsInBook[book->bookSize] = passingCard;
    book->bookSize++;
}

static void transferBookCards(game_state *game, player *player, values bookValue) {
    book newBook;
    newBook.bookSize = 0;
    newBook.ownerId = player->playerNum;
    newBook.bookValue = bookValue;

    for (int i = player->handSize - 1; i >= 0; i--) {
        if (player->hand[i].value == bookValue) {
            giveCardToBook(game, &newBook, player->playerNum, i);
        }
    }
    newBook.completed = true;
    if (game->sizeOfBooks >= 13) {
        fprintf(stderr, "Error: exceeding max number of books");
        exit(EXIT_FAILURE);
    }
    game->books[game->sizeOfBooks] = newBook;
    game->sizeOfBooks++;
}

static void checkHandForBook(game_state *game, player *player) {

    int possibleValues[13] = {0};

    for (int i = 0; i < player->handSize; i++) {
        possibleValues[player->hand[i].value]++;

        if (possibleValues[player->hand[i].value] == 4) {
            transferBookCards(game, player, player->hand[i].value);
            return;
        }
    }
}

static void checkPlayersForBook(game_state *game) {
    for (int i = 0; i < game->playerCount; i++) {
        checkHandForBook(game, &game->players[i]);
    }
}

static bool checkHandForCard(player *target, player *asker, values targetedValue) {
    bool found = false;

    for (int i = target->handSize - 1; i >= 0; i--) {
        if (target->hand[i].value == targetedValue) {
            giveCardToPlayer(target, asker, i);
            found = true;
        }
    }

    return found;
}

static bool checkForWin(game_state *game) {
    if (game->sizeOfBooks == 13) {
        game->winCondition.hasWon = true;
    } else {
        return false;
    }

    int bookCount[game->playerCount];
    for (int i = 0; i < game->playerCount; i++) {
        bookCount[i] = 0;
    }

    for (int bookIndex = 0; bookIndex < game->sizeOfBooks; bookIndex++) {
        for (int playerIndex = 0; playerIndex < game->playerCount; playerIndex++) {
            if (game->books[bookIndex].ownerId == game->players[playerIndex].playerNum) {
                bookCount[playerIndex]++;
                break;
            }
        }
    }

    int id;
    int lastBookSize = 0;
    for (int i = 0; i < game->playerCount; i++) {
        if (bookCount[i] > lastBookSize) {
            id = i;
            lastBookSize = bookCount[i];
        }
    }

    game->winCondition.winnerId = id;
    return game->winCondition.hasWon;
}

void organizeHand(player *player) {
    for (int i = 1; i < player->handSize; i++) {
        card current = player->hand[i];
        int j = i;

        while (j > 0 && player->hand[j - 1].value < current.value) {
            player->hand[j] = player->hand[j - 1];
            j--;
        }

        player->hand[j] = current;
    }
}

void gameInit() {
    g_game.sizeOfBooks = 0;
    g_game.winCondition.hasWon = false;
    g_game.playerCount = 2;

    shuffleDeck(g_deck);
    deckToStack(&g_game, g_deck);

    playersInit(&g_game);

    for (int i = 0; i < G_STARTING_HAND_SIZE; i++) {
        drawCard(&g_game, &g_game.players[0]);
        drawCard(&g_game, &g_game.players[1]);
    }
}

static void gameplayLoop(game_state *game) {
    while (!game->winCondition.hasWon) {
        tui_clearScreen();
        checkPlayersForBook(game);
        tui_displayTurn(game);

        for (int playerIndex = 0; playerIndex < game->playerCount; playerIndex++) {
            printf("Player %d's turn\n", playerIndex + 1);

            bool gotCard = false;

            // TEMPORARY SOLUTION FOR FINDING OTHER PLAYER FOR HAND CHECKS
            // NEEDS TO SCALE WITH MULTIPLE PLAYERS
            int otherPlayerId;
            if (playerIndex == 0) {
                otherPlayerId = 1;
            } else {
                otherPlayerId = 0;
            }

            // ASK FOR INPUT BLOCK
            char buffer[4]; // has to be five cuz it maybe at somepoint holds the \n
            if (!tui_askForCard(&game->players[playerIndex], buffer, sizeof(buffer))) {
                printf("Exited, ending game\n");
                return;
            }
            values inputedValue = shorthandToValues(buffer);

            // CHECK HAND BLOCK
            // check if card/cards is in targets hand, if it is transfer cards
            if (checkHandForCard(&game->players[otherPlayerId], &game->players[playerIndex],
                                 inputedValue)) {
                gotCard = true;
            }

            while (gotCard) {
                checkPlayersForBook(game);
                if (checkForWin(game)) {
                    tui_winScreen(game);
                    return;
                }
                tui_displayTurn(game);

                char buffer[4];

                if(!tui_askForCard(&game->players[playerIndex], buffer, sizeof(buffer))) {
                    printf("Exited, ending game\n");
                    return;
                }
                values inputedValue = shorthandToValues(buffer);

                if (checkHandForCard(&game->players[otherPlayerId], &game->players[playerIndex],
                                     inputedValue)) {
                    gotCard = true;
                } else {
                    gotCard = false;
                }
            }

            // DRAW CARD BLOCK
            checkPlayersForBook(game);
            if (checkForWin(game)) {
                tui_winScreen(game);
                return;
            }
            tui_displayTurn(game);
            drawCard(game, &game->players[playerIndex]);
            checkPlayersForBook(game);
            if (checkForWin(game)) {
                tui_winScreen(game);
                return;
            }
            tui_displayTurn(game);
        }
    }

    printf("Game Won!\n");
    printf("Winner: player %d\n", game->winCondition.winnerId);
}

void startGame() {
    tui_clearScreen();
    tui_startScreen();

    tui_displayTurn(&g_game);

    gameplayLoop(&g_game);
}