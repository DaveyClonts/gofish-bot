#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "deck.h"
#include "tui.h"

// steps to game
// dealer can be the computer
// for 2 players 7 cards dealt to each player

// End condition: all thirteen books have been won
// Books can belong to a player
// Players: 1 human 1 bot (for now)

const int G_STARTING_HAND_SIZE = 7;
const int G_PLAYER_COUNT = 2; //eventually will be mutable as a field of game_state struct

game_state g_game;

void deckToStack(game_state *game, card deck[]) {
    stackInit(&game->drawPile);

    for (int i = 0; i < DECK_SIZE; i++) {
        stackPush(&game->drawPile, deck[i]);
    }
}

void playersInit(game_state *game) {
    for(int i = 0; i < G_PLAYER_COUNT; i++) {
        game->players[i].playerNum = i;
        game->players[i].handSize = 0;
        game->players[i].capacity = G_STARTING_HAND_SIZE;
        game->players[i].hand = malloc(game->players[i].capacity * sizeof(card));
        //potential for error handling and asserts on capacity
    }
}

void checkHandCapacity(player *player) {
    if (player->handSize >= player->capacity) {
        player->capacity *= 2;
        player->hand = realloc(player->hand,player->capacity * sizeof(card));
    }
}

void drawCard(game_state *game, int playerId) {
    player *currentPlayer = &game->players[playerId];

    checkHandCapacity(currentPlayer);

    if (stackPop(&game->drawPile,&currentPlayer->hand[currentPlayer->handSize])) {
        currentPlayer->handSize++;
    }
}

void giveCardToPlayer(game_state *game, int giverId, int recieverId, int cardIndex) {
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

void giveCardToBook(game_state *game, book *book, int giverId, int cardIndex) {
    player *giver = &game->players[giverId];

    card passingCard = giver->hand[cardIndex];
    giver->handSize--;
    book->cardsInBook[book->bookSize] = passingCard;
    book->bookSize++;
}

void giveBookToGame(game_state *game, book passingBook) {
    &game->books[game->sizeOfBooks] = passingBook;
    &game->sizeOfBooks++;
}

//TODO: func for organizing hand by putting like values together

void transferBookCards(game_state *game, player *player, int bookValue) {
    book newBook;
    newBook.bookSize = 0;
    newBook.ownerId = player->playerNum;

    for (int i = 0; i < player->handSize; i++) {
        if (player->hand[i].value == bookValue) {
            giveCardToBook(game, &newBook, player->playerNum, i);
        }
        //giveBookToGame()
    }
}

void checkHandForBook(game_state *game, player *player) {

    int possibleValues[13] = {0};

    for (int i = 0; i < player->handSize; i++) {
        possibleValues[player->hand[i].value]++;
        

        if (possibleValues[player->hand[i].value] == 4) {
            //transferBookCards();
        }
    }
}

void checkPlayersForBook(game_state *game) {
    for (int i = 0; i < G_PLAYER_COUNT; i++) {
        checkHandForBook(&game->players[i]);
    }
}

void gameInit() {
    shuffleDeck(g_deck);
    deckToStack(&g_game, g_deck);

    playersInit(&g_game);
    g_game.sizeOfBooks = 0;

    for (int i = 0; i < G_STARTING_HAND_SIZE; i++) {
        drawCard(&g_game, 0);
        drawCard(&g_game, 1);

        // TEST
        // printf("Player 1's hand: \n");
        // for(int i = 0; i < player1.handSize; i++) {
        //     readCard(player1.hand[i]);
        // }
        // printf("\n");

        // printf("Player 2's hand: \n");
        // for(int i = 0; i < player2.handSize; i++) {
        //     readCard(player2.hand[i]);
        // }
        // printf("\n");

        // printf("Size of draw pile: \n");
        // printf("%d \n", drawPile.size);
        // printf("----------------- \n");
    }
}

void gameplayLoop(game_state *game) {
    while(!game->winCondition.hasWon) {

    }
}

void startGame() {
    tui_displayHands();
}