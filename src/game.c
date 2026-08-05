#include "game.h"
#include "deck.h"
#include "event_stream.h"
#include "player.h"
#include "stack.h"
#include "tui.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GameState g_game;
EventStream g_eventStream;

static void deckToStack(GameState *game, Card deck[]) {
    stackInit(&game->drawPile);

    for (int i = 0; i < DECK_SIZE; i++) {
        stackPush(&game->drawPile, deck[i]);
    }
}

static Card drawCard(GameState *game, Player *player) {
    player_checkHandCapicity(player);

    Card drawnCard;
    if (stackPop(&game->drawPile, &player->hand[player->handSize])) {
        drawnCard = player->hand[player->handSize];
        player->handSize++;

        publishEvent(&g_eventStream,
            (Event){
                .eventType = CARD_DRAWN,
                .actorId = player->playerNum,
                .value = drawnCard.value,
            });
    }

    return drawnCard;
}

static void giveCardToBook(GameState *game, Book *book, int giverId, int cardIndex) {
    Player *giver = &game->players[giverId];

    Card passingCard = giver->hand[cardIndex];

    for (int i = cardIndex; i < giver->handSize - 1; i++) {
        giver->hand[i] = giver->hand[i + 1];
    }

    giver->handSize--;
    book->cardsInBook[book->bookSize] = passingCard;
    book->bookSize++;
}

static void transferBookCards(GameState *game, Player *player, values bookValue) {
    Book newBook;
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

static void checkHandForBook(GameState *game, Player *player) {

    int possibleValues[13] = {0};

    for (int i = 0; i < player->handSize; i++) {
        possibleValues[player->hand[i].value]++;

        if (possibleValues[player->hand[i].value] == 4) {
            values bookValue = player->hand[i].value;
            publishEvent(&g_eventStream,
                (Event){
                    .eventType = BOOK_FOUND,
                    .actorId = player->playerNum,
                    .value = bookValue,
                });

            transferBookCards(game, player, bookValue);
            return;
        }
    }
}

static void checkPlayersForBook(GameState *game) {
    for (int i = 0; i < game->playerCount; i++) {
        checkHandForBook(game, &game->players[i]);
    }
}

static bool checkForWin(GameState *game) {
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

            publishEvent(&g_eventStream,
                (Event){
                    .eventType = GAME_WON,
                    .actorId = game->players[i].playerNum,
                });
        }
    }

    return game->winCondition.hasWon;
}

static void initPlayers(GameState *game) {

    for (int i = 0; i < game->playerCount; i++) {
        player_initPlayer(&game->players[i], i);
    }

    game->players[0].isUser = true; // forced for now
}

static values takeInput(Player *player) {
    char buffer[4];
    if (!tui_askForCard(player, buffer, sizeof(buffer))) {
        printf("Exited, ending game\n");
        exit(EXIT_FAILURE);
    }
    values inputedValue = shorthandToValues(buffer);
    return inputedValue;
}

static bool checkHandForCard(Player *actor, Player *target, values targetedValue) {
    bool gotCard = false;
    for (int i = target->handSize - 1; i >= 0; i--) {
        if (target->hand[i].value == targetedValue) {
            publishEvent(&g_eventStream,
                (Event){.eventType = CARD_TRANSFERRED,
                    .actorId = actor->playerNum,
                    .value = targetedValue,
                    .targetId = target->playerNum});
            player_giveCardToPlayer(target, actor, i);
            gotCard = true;
        }
    }

    return gotCard;
}

static values emptyHand(GameState *game, Player *drawingPlayer) {
    publishEvent(
        &g_eventStream, (Event){.eventType = EMPTY_HAND, .actorId = drawingPlayer->playerNum});

    Card card;
    card = drawCard(game, drawingPlayer);
    return card.value;
}

static void requestCard(GameState *game, Player *actor, Player *target) {

    // ASK FOR CARD
    values inputedValue;
    if (actor->handSize == 0) {
        inputedValue = emptyHand(game, actor);
    } else {
        inputedValue = takeInput(actor);
        publishEvent(&g_eventStream, (Event){.eventType = CARD_REQUESTED, .value = inputedValue});
    }

    // CHECK HAND FOR CARD
    bool gotCard = false;
    if (checkHandForCard(actor, target, inputedValue)) {
        gotCard = true;
    }

    while (gotCard) {
        checkPlayersForBook(game);
        if (checkForWin(game)) {
            tui_winScreen(game);
            return;
        }
        tui_displayTurn(game);

        values nextValue = takeInput(actor);
        if (checkHandForCard(actor, target, nextValue)) {
            gotCard = true;
        } else {
            gotCard = false;
        }
    }

    // GO FISH
    publishEvent(&g_eventStream,
        (Event){
            .eventType = GO_FISH,
        });
    if (!stackIsEmpty(&game->drawPile)) {
        drawCard(game, actor);
        checkPlayersForBook(game);
        if (checkForWin(game)) {
            tui_winScreen(game);
            return;
        }
    }
}

void gameInit() {
    g_game.sizeOfBooks = 0;
    g_game.winCondition.hasWon = false;
    g_game.playerCount = 2;
    g_game.eventLog[0] = '\0';

    shuffleDeck(g_deck);
    deckToStack(&g_game, g_deck);

    initPlayers(&g_game);
    eventStreamInit(&g_eventStream);

    for (int i = 0; i < G_STARTING_HAND_SIZE; i++) {
        drawCard(&g_game, &g_game.players[0]);
        drawCard(&g_game, &g_game.players[1]);
    }
}

static void gameplayLoop(GameState *game) {
    while (!game->winCondition.hasWon) {
        for (int playerIndex = 0; playerIndex < game->playerCount; playerIndex++) {
            publishEvent(&g_eventStream,
                (Event){
                    .eventType = TURN_STARTED,
                    .actorId = game->players[playerIndex].playerNum,
                });

            checkPlayersForBook(game);
            tui_displayTurn(game);
            printf("Player %d's turn\n", playerIndex + 1);

            // TEMPORARY SOLUTION FOR FINDING OTHER PLAYER FOR HAND CHECKS
            // NEEDS TO SCALE WITH MULTIPLE PLAYERS
            int otherPlayerId;
            if (playerIndex == 0) {
                otherPlayerId = 1;
            } else {
                otherPlayerId = 0;
            }

            requestCard(&g_game, &game->players[playerIndex], &game->players[otherPlayerId]);
        }
    }
}

void startGame() {
    tui_clearScreen();
    tui_startScreen();

    tui_displayTurn(&g_game);

    gameplayLoop(&g_game);
}
