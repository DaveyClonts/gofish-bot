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

void giveCard(game_state *game, int giverId, int recieverId, int cardIndex) {
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

void gameInit() {
    shuffleDeck(g_deck);
    deckToStack(&g_game, g_deck);

    playersInit(&g_game);

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

void gameplayLoop() {

}

void startGame() {
    tui_displayHands();
}