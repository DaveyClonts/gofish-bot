#include "game.h"
#include "deck.h"
#include "player.h"
#include "stack.h"
#include "tui.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

game_state g_game;

static void deckToStack(game_state *game, card deck[]) {
    stackInit(&game->drawPile);

    for (int i = 0; i < DECK_SIZE; i++) {
        stackPush(&game->drawPile, deck[i]);
    }
}

static card drawCard(game_state *game, player *player) {

    player_checkHandCapicity(player);

    card drawnCard;
    if (stackPop(&game->drawPile, &player->hand[player->handSize])) {
        drawnCard = player->hand[player->handSize];
        player->handSize++;
    }

    return drawnCard;
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
            strcpy(game->eventBuffer, "Book found!\n");
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

static bool checkForWin(game_state *game) {
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

    // TODO: make this scale with more then two players
    for (int i = 0; i < game->playerCount; i++) {
        if (bookCount[i] > 6) {
            game->winCondition.hasWon = true;
            game->winCondition.winnerId = i;
        }
    }

    return game->winCondition.hasWon;
}

static void initPlayers(game_state *game) {

    for (int i = 0; i < game->playerCount; i++) {
        player_initPlayer(&game->players[i], i);
    }

    game->players[0].isUser = true; // forced for now
}

static values takeInput(player *player) {
    char buffer[4];
    if (!tui_askForCard(player, buffer, sizeof(buffer))) {
        printf("Exited, ending game\n");
        return NULL;
    }
    values inputedValue = shorthandToValues(buffer);
    return inputedValue;
}

static values emptyHand(game_state *game, player *drawingPlayer) {
    card card;
    card = drawCard(game, drawingPlayer);
    return card.value;
}

void gameInit() {
    g_game.sizeOfBooks = 0;
    g_game.winCondition.hasWon = false;
    g_game.playerCount = 2;
    g_game.eventBuffer[0] = '\0';

    shuffleDeck(g_deck);
    deckToStack(&g_game, g_deck);

    initPlayers(&g_game);

    for (int i = 0; i < G_STARTING_HAND_SIZE; i++) {
        drawCard(&g_game, &g_game.players[0]);
        drawCard(&g_game, &g_game.players[1]);
    }
}

static void gameplayLoop(game_state *game) {
    while (!game->winCondition.hasWon) {
        for (int playerIndex = 0; playerIndex < game->playerCount; playerIndex++) {
            checkPlayersForBook(game);
            tui_displayTurn(game);
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

            // ASK FOR CARD BLOCK
            values inputedValue;
            if (game->players[playerIndex].handSize == 0) {
                strcpy(game->eventBuffer, "Empty hand, drawing and requesting drawn card");
                inputedValue = emptyHand(game, &game->players[playerIndex]);
            } else {
                inputedValue = takeInput(&game->players[playerIndex]);
            }

            // CHECK HAND BLOCK
            // check if card/cards is in targets hand, if it is transfer cards
            if (player_checkHandForCard(&game->players[otherPlayerId], &game->players[playerIndex],
                                        inputedValue)) {
                gotCard = true;
                strcpy(game->eventBuffer, "Card found!\n");
            }

            while (gotCard) {
                checkPlayersForBook(game);
                if (checkForWin(game)) {
                    tui_winScreen(game);
                    return;
                }
                tui_displayTurn(game);

                values inputedValue = takeInput(&game->players[playerIndex]);

                if (player_checkHandForCard(&game->players[otherPlayerId],
                                            &game->players[playerIndex], inputedValue)) {
                    gotCard = true;
                    strcpy(game->eventBuffer, "Card found!\n");
                } else {
                    gotCard = false;
                }
            }

            // DRAW CARD BLOCK
            if (!stackIsEmpty(&game->drawPile)) {
                drawCard(game, &game->players[playerIndex]);
                strcpy(game->eventBuffer, "Card drawn\n");
                checkPlayersForBook(game);
                if (checkForWin(game)) {
                    tui_winScreen(game);
                    return;
                }
            }
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