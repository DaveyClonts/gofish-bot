#ifndef PLAYER_H
#define PLAYER_H
#include "deck.h"
#include <stdbool.h>

#define G_STARTING_HAND_SIZE 7

typedef struct {
    int playerNum;
    card *hand;
    int handSize;
    int capacity;
    bool isUser;
} player;

void player_initPlayer(player *player, int playerId);

void player_checkHandCapicity(player *player);

void player_organizeHand(player *player);

void player_giveCardToPlayer(player *giver, player *reciever, int cardIndex);

bool player_checkHandForCard(player *target, player *asker, values targetedValue);

#endif