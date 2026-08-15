#ifndef PLAYER_H
#define PLAYER_H
#include "deck.h"
#include <stdbool.h>

#define G_STARTING_HAND_SIZE 7

typedef struct {
    int playerNum;
    Card *hand;
    int handSize;
    int capacity;
    bool isUser;
} Player;

void player_initPlayer(Player *player, int playerId);

void player_checkHandCapicity(Player *player);

void player_organizeHand(Player *player);

void player_giveCardToPlayer(Player *giver, Player *reciever, int cardIndex);

#endif
