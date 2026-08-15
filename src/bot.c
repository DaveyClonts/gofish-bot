#include "bot.h"
#include "event_stream.h"
#include "game.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>

static void checkMemoryCapacity(Memory *memory) {
    if (memory->size >= memory->capacity) {
        memory->capacity *= 2;
        CertainCard *newRealloc =
            realloc(memory->certainCards, memory->capacity * sizeof(CertainCard));

        if (newRealloc == NULL) {
            fprintf(stderr, "Error: memory realloc failed");
            exit(EXIT_FAILURE);
        }

        memory->certainCards = newRealloc;
    }
}

static void checkBotManagerCapacity(BotManager *bots) {
    if (bots->size >= bots->capacity) {
        bots->capacity *= 2;

        BotState *newRealloc = realloc(bots->bots, bots->capacity * sizeof(BotState));

        if (newRealloc == NULL) {
            fprintf(stderr, "Error: bot size realloc failed");
            exit(EXIT_FAILURE);
        }

        bots->bots = newRealloc;
    }
}

// this func assumes that event has an actorId and value
static void insertMemory(BotState *bot, Event event) {

    // checks that memory doesnt already exist
    for (int i = 0; i < bot->memory.size; i++) {
        if (bot->memory.certainCards[i].targetId == event.actorId &&
            bot->memory.certainCards[i].cardValue == event.value) {
            return;
        }
    }

    CertainCard newCard;
    newCard.cardValue = event.value;
    newCard.targetId = event.actorId;

    checkMemoryCapacity(&bot->memory);
    bot->memory.certainCards[bot->memory.size] = newCard;
    bot->memory.size++;
}

// this func assumes that event has a targetId and value
static void deleteMemory(BotState *bot, int targetId, values value) {
    for (int i = 0; i < bot->memory.size; i++) {
        if (bot->memory.certainCards[i].targetId == targetId &&
            bot->memory.certainCards[i].cardValue == value) {
            for (int j = i; j < bot->memory.size - 1; j++) {
                bot->memory.certainCards[j] =
                    bot->memory.certainCards[j + 1]; // shifts array to left deleting given index
            }

            bot->memory.size--;
            return;
        }
    }
}

static void loadMemory(BotState *bot, EventStream *stream) {
    for (int i = bot->processedMemory; i < stream->size; i++) {
        if (stream->events[i].eventType == CARD_REQUESTED) {
            insertMemory(bot, stream->events[i]);
        } else if (stream->events[i].eventType == CARD_TRANSFERRED) {
            deleteMemory(bot, stream->events[i].targetId, stream->events[i].value);
        } else if (stream->events[i].eventType == BOOK_FOUND) {
            deleteMemory(bot, stream->events[i].actorId, stream->events[i].value);
        }
    }

    bot->processedMemory = stream->size;
}

void initBotManager(BotManager *bots) {
    bots->size = 0;
    bots->capacity = 1;
    bots->bots = malloc(bots->capacity * sizeof(BotState));

    if (bots->bots == NULL) {
        fprintf(stderr, "Error: bot manager malloc failed");
        exit(EXIT_FAILURE);
    }
}

void initBot(BotManager *bots, int playerId) {

    BotState newBot = {0};
    newBot.playerId = playerId;
    newBot.processedMemory = 0;
    newBot.memory.size = 0;
    newBot.memory.capacity = 8;
    CertainCard *newMalloc = malloc(newBot.memory.capacity * sizeof(CertainCard));

    if (newMalloc == NULL) {
        fprintf(stderr, "Error: certain cards malloc failed");
        exit(EXIT_FAILURE);
    }

    newBot.memory.certainCards = newMalloc;

    checkBotManagerCapacity(bots);
    bots->bots[bots->size] = newBot;
    bots->size++;
}

BotState *getBot(BotManager *bots, int playerId) {
    for (int i = 0; i < bots->size; i++) {
        if (bots->bots[i].playerId == playerId) {
            return &bots->bots[i];
        }
    }

    return NULL;
}

static bool botAskForCard(GameState *game, int actorId, int targetId, values requestedValue) {
    Player *asker = &game->players[actorId];
    Player *target = &game->players[targetId];

    publishEvent(&game->stream,
        (Event){
            .eventType = CARD_REQUESTED,
            .value = requestedValue,
            .actorId = asker->playerNum,
        });

    if (checkHandForCard(asker, target, requestedValue)) {
        checkPlayersForBook(game);
        if (checkForWin(game)) {
            tui_winScreen(game);
        }
        tui_displayTurn(game);
        tui_waitForKey();
        return true;
    } else {
        tui_displayTurn(game);
        tui_waitForKey();
        return false;
    }
}

void doTurn(BotState *bot, GameState *game) {
    loadMemory(bot, &game->stream);
    Player *player = &game->players[bot->playerId];

    if (player->handSize == 0) {
        // empty hand logic
    }

    // if u have certain cards in hand, ask for them
    for (int i = 0; i < bot->memory.size; i++) {

        // never ask yourself
        if (bot->memory.certainCards[i].targetId == bot->playerId) {
            continue;
        }

        for (int j = 0; j < player->handSize; j++) {
            if (bot->memory.certainCards[i].cardValue == player->hand[j].value) {
                CertainCard cardToRequest = bot->memory.certainCards[i];
                botAskForCard(
                    game, player->playerNum, cardToRequest.targetId, cardToRequest.cardValue);
                return;
            }
        }
    }

    // else ask for the card you have either
    // 1. not asked for yet at paticular target
    // 2. been the longest since you asked for it

    // TEMP LOGIC, WILL WRITE LOGIC THAT DOES THE ABOVE
    botAskForCard(game, player->playerNum, 1, player->hand[0].value);
}
