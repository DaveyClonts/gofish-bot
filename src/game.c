#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "deck.h"

// steps to game
// dealer can be the computer
// for 2 players 7 cards dealt to each player

// End condition: all thirteen books have been won
// Books can belong to a player
// Players: 1 human 1 bot (for now)

stack_card drawPile; //hopeful this is okay global
const int STARTING_HAND_SIZE = 7;

void deckToStack(card deck[]) {
    stackInit(&drawPile);

    for (int i = 0; i < DECK_SIZE; i++) {
        stackPush(&drawPile, deck[i]);
    }
}

void playerInit(int playerNum, player *player) {
    player->playerNum = playerNum;
    player->handSize = 0;
    player->handMemoryAllocated = STARTING_HAND_SIZE;
    player->hand = malloc(player->handMemoryAllocated * sizeof(card)); 
}

void drawCard(stack_card *drawPile, player *player) {

    if(player->handSize >= player->handMemoryAllocated) {
        player->handMemoryAllocated *= 2;
        player->hand = realloc(player->hand, player->handMemoryAllocated * sizeof(card));
    }

    stackPop(drawPile, &player->hand[player->handSize++]);
}

void gameInit() {
    shuffleDeck(deck);
    deckToStack(deck);

    player player1;
    player player2;

    playerInit(1, &player1);
    playerInit(2, &player2);

    for (int i = 0; i < STARTING_HAND_SIZE; i++) {
        drawCard(&drawPile, &player1);
        drawCard(&drawPile, &player2);

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
