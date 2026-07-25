#include "player.h"
#include <stdio.h>
#include <stdlib.h>

void player_initPlayer(player *player, int playerId) {
    player->playerNum = playerId;
    player->handSize = 0;
    player->capacity = G_STARTING_HAND_SIZE;
    player->hand = malloc(player->capacity * sizeof(card));

    if (player->hand == NULL) {
        fprintf(stderr, "Error: memory allocation failed");
        exit(EXIT_FAILURE);
    }
}

void player_checkHandCapicity(player *player) {
    if (player->handSize >= player->capacity) {
        player->capacity *= 2;
        card *reallocHand = realloc(player->hand, player->capacity * sizeof(card));

        if (reallocHand == NULL) {
            fprintf(stderr, "Error: hand realloc failed");
            exit(EXIT_FAILURE);
        }

        player->hand = reallocHand;
    }
}

void player_organizeHand(player *player) {
    for (int i = 1; i < player->handSize; i++) {
        card current = player->hand[i];
        int j = i;

        while (j > 0 && player->hand[j - 1].value < current.value) {
            player->hand[j] = player->hand[j - 1];
            j--;
        }

        player->hand[j] = current;
    }
}

void player_giveCardToPlayer(player *giver, player *reciever, int cardIndex) {
    if (giver->handSize <= 0) {
        fprintf(stderr, "Error: giver's hand is empty");
        exit(EXIT_FAILURE);
    }

    if (cardIndex < 0 || cardIndex >= giver->handSize) {
        fprintf(stderr, "Error: cardIndex is out of bounds");
        exit(EXIT_FAILURE);
    }

    // shifts cards left and reduces handSize
    card passingCard = giver->hand[cardIndex];
    for (int i = cardIndex; i < giver->handSize - 1; i++) {
        giver->hand[i] = giver->hand[i + 1];
    }
    giver->handSize--;

    player_checkHandCapicity(reciever);
    reciever->hand[reciever->handSize] = passingCard;
    reciever->handSize++;
}

bool player_checkHandForCard(player *target, player *asker, values targetedValue) {
    bool found = false;

    for (int i = target->handSize - 1; i >= 0; i--) {
        if (target->hand[i].value == targetedValue) {
            player_giveCardToPlayer(target, asker, i);
            found = true;
        }
    }

    return found;
}