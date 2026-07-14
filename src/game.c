#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "deck.h"
#include "tui.h"

// End condition: all thirteen books have been won
// Worried that this file needs to be abstracted into player, books and what not

const int G_STARTING_HAND_SIZE = 7;

game_state g_game;

static void deckToStack(game_state *game, card deck[]) {
    stackInit(&game->drawPile);

    for (int i = 0; i < DECK_SIZE; i++) {
        stackPush(&game->drawPile, deck[i]);
    }
}

static void playersInit(game_state *game) {
    for(int i = 0; i < game->playerCount; i++) {
        game->players[i].playerNum = i;
        game->players[i].handSize = 0;
        game->players[i].capacity = G_STARTING_HAND_SIZE;
        game->players[i].hand = malloc(game->players[i].capacity * sizeof(card));
        //potential for error handling and asserts on capacity
    }
    game->players[0].isUser = true; //forced for now
}

static void checkHandCapacity(player *player) {
    if (player->handSize >= player->capacity) {
        player->capacity *= 2;
        player->hand = realloc(player->hand,player->capacity * sizeof(card));
    }
}

static void drawCard(game_state *game, int playerId) {
    player *currentPlayer = &game->players[playerId];

    checkHandCapacity(currentPlayer);

    if (stackPop(&game->drawPile,&currentPlayer->hand[currentPlayer->handSize])) {
        currentPlayer->handSize++;
    }
}

static void giveCardToPlayer(game_state *game, int giverId, int recieverId, int cardIndex) {
    player *giver = &game->players[giverId];
    player *reciever = &game->players[recieverId];
    if (giver->handSize <= 0) {
        fprintf(stderr, "Error: giver's hand is empty");
        exit(EXIT_FAILURE);
    }

    //shifts cards left and reduces handSize
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
    if(game->sizeOfBooks >= 13) {
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

//unused? eventually goes in gameplay loop
static void checkPlayersForBook(game_state *game) {
    for (int i = 0; i < game->playerCount; i++) {
        checkHandForBook(game,&game->players[i]);
    }
}

//TODO: func for organizing hands by like value cards

void gameInit() {
    g_game.sizeOfBooks = 0;
    g_game.winCondition.hasWon = false;
    g_game.playerCount = 2;

    shuffleDeck(g_deck);
    deckToStack(&g_game, g_deck);

    playersInit(&g_game);

    for (int i = 0; i < G_STARTING_HAND_SIZE; i++) {
        drawCard(&g_game, 0);
        drawCard(&g_game, 1);
    }
}

static void gameplayLoop(game_state *game) {
    while(!game->winCondition.hasWon) {
        tui_clearScreen();
        tui_displayTurn(game);

        for(int i = 0; i < game->playerCount; i++) {
            bool gotCard;

            //has to be five cuz it maybe at somepoint holds the \n 
            char playerInput[5];
            tui_askForCard(playerInput, sizeof(playerInput));

            while(gotCard) {

                

                //ask for card
                //do action
                //check for win
                //refresh display
                //if got card loop back to ask card
                //else break loop and do next player
            }
        }

    }
}

void startGame() {
    tui_clearScreen();
    tui_startScreen();

    tui_displayTurn(&g_game);

    char playerInput[4];
    tui_askForCard(playerInput, sizeof(playerInput));

    //gameplayLoop(&g_game);
}