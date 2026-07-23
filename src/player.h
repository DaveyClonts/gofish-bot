#ifndef PLAYER_H
#define PLAYER_H
#include "game.h"

void player_initPlayer(player *player, int playerId);

void player_checkHandCapicity(player *player);

void player_organizeHand(player *player);

void player_giveCardToPlayer(player *giver, player *reciever, int cardIndex);

bool player_checkHandForCard(player *target, player *asker, values targetedValue);

#endif