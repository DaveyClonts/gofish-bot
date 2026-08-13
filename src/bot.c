#include "bot.h"
#include "event_stream.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

BotState g_bots; // this will prob actually be in game

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
        bots->capacity += 1;

        BotState *newRealloc = realloc(bots->bots, bots->capacity * sizeof(BotState));

        if (newRealloc == NULL) {
            fprintf(stderr, "Error: bot size realloc failed");
            exit(EXIT_FAILURE);
        }

        bots->bots = newRealloc;
    }
}

// this func assumes that event has an actorId and value
static void insertMemory(Event event) {

    // checks that memory doesnt already exist
    for (int i = 0; i < g_bot.memory.size; i++) {
        if (g_bot.memory.certainCards[i].targetId == event.actorId &&
            g_bot.memory.certainCards[i].cardValue == event.value) {
            return;
        }
    }

    CertainCard newCard;
    newCard.cardValue = event.value;
    newCard.targetId = event.actorId;

    checkMemoryCapacity(&g_bot.memory);
    g_bot.memory.certainCards[g_bot.memory.size] = newCard;
    g_bot.memory.size++;
}

// this func assumes that event has a targetId and value
static void deleteMemory(int targetId, values value) {
    for (int i = 0; i < g_bot.memory.size; i++) {
        if (g_bot.memory.certainCards[i].targetId == targetId &&
            g_bot.memory.certainCards[i].cardValue == value) {
            for (int j = i; j < g_bot.memory.size - 1; j++) {
                g_bot.memory.certainCards[j] =
                    g_bot.memory.certainCards[j + 1]; // shifts array to left deleting given index
            }

            g_bot.memory.size--;
            return;
        }
    }
}

static void loadMemory(EventStream *stream) {
    // loop through stream
    // find certain cards
    // update certain cards if transfered

    for (int i = g_bot.processedMemory; i < stream->size; i++) {
        if (stream->events[i].eventType == CARD_REQUESTED) {
            insertMemory(stream->events[i]);
        } else if (stream->events[i].eventType == CARD_TRANSFERRED) {
            deleteMemory(stream->events[i].targetId, stream->events[i].value);
        } else if (stream->events[i].eventType == BOOK_FOUND) {
            deleteMemory(stream->events[i].actorId, stream->events[i].value);
        }
    }

    g_bot.processedMemory = stream->size;
}

void initBot(BotManager *bots, int playerId) {

    BotState newBot;
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
    bots->size++;
    bots->bots[bots->size] = newBot;
}

void doTurn(GameState *game) {
    loadMemory(&game->stream);
    Player bot = game->players[g_bot.botId];

    // if u have certain cards in hand, ask for them
    for (int i = 0; i < g_bot.memory.size; i++) {

        // never ask yourself
        if (g_bot.memory.certainCards[i].targetId == g_bot.botId) {
            continue;
        }

        for (int j = 0; j < bot.handSize; j++) {
            if (g_bot.memory.certainCards[i].cardValue == bot.hand[j].value) {
                // request that card
                return;
            }
        }
    }

    // else ask for the card you have either
    // 1. not asked for yet at paticular target
    // 2. been the longest since you asked for it
}