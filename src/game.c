#include "game.h"
#include <stdbool.h>
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

void deckToStack(card deck[]) {
    stackInit(&g_game.drawPile);

    for (int i = 0; i < DECK_SIZE; i++) {
        stackPush(&g_game.drawPile, deck[i]);
    }
}

void playerInit(game_state *game) {
    for(int i = 0; i < G_PLAYER_COUNT; i++) {
        game->players[i].playerNum = i;
        game->players[i].handSize = 0;
        game->players[i].capacity = G_STARTING_HAND_SIZE;
        game->players[i].hand = malloc(game->players[i].capacity * sizeof(card));
        //potential for error handling and asserts on capacity
    }
}

// void drawCard(stack_card *drawPile, player *player) {

//     if(player->handSize >= player->capacity) {
//         player->capacity *= 2;
//         player->hand = realloc(player->hand, player->capacity * sizeof(card));
//     }

//     stackPop(drawPile, &player->hand[player->handSize++]);
// }

// void drawCard(game_state *game, int playerId) {
//     if(game->players[playerId].handSize >= game->players[playerId].capacity) {
//         game->players[playerId].capacity *= 2;
//         game->players[playerId].hand = realloc(game->players[playerId].hand, game->players[playerId].capacity * sizeof(card));
//     }

//     if(
//         stackPop(
//             &game->drawPile,
//             &game->players[playerId].hand[game->players[playerId].handSize] //sorry
//         )
//     ) {
//         game->players[playerId].handSize++;
//     }

// }

void drawCard(game_state *game, int playerId) {
    player *currentPlayer = &game->players[playerId];

    if (currentPlayer->handSize >= currentPlayer->capacity) {
        currentPlayer->capacity *= 2;
        currentPlayer->hand = realloc(
            currentPlayer->hand,
            currentPlayer->capacity * sizeof(card)
        );
    }

    if (stackPop(&game->drawPile,&currentPlayer->hand[currentPlayer->handSize])) {
        currentPlayer->handSize++;
    }
}


void gameInit() {
    shuffleDeck(deck); //TODO: ask chat about better way to do global deck
    deckToStack(deck);

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