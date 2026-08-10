#include "bot.h"
#include "event_stream.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

BotState g_bot;

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

// this func assumes that event has a actorId and value
static void insertMemory(Event event) {
    CertainCard newCard;
    newCard.cardValue = event.value;
    newCard.targetId = event.actorId;

    checkMemoryCapacity(&g_bot.memory);
    g_bot.memory.certainCards[g_bot.memory.size] = newCard;
    g_bot.memory.size++;
}

static void loadMemory(EventStream *stream, Memory memory) {
    // loop through stream
    // find certain cards
    // update certain cards if transfered

    for (int i = g_bot.processedMemory; i < stream->size; i++) {
        if (stream->events[i].eventType == CARD_REQUESTED) {
            insertMemory(stream->events[i]);
        } //TODO: update logic
    }
}

void initBot(BotState *bot, int botId) {
    bot->botId = botId;
    bot->processedMemory = 0;
    bot->memory.size = 0;
    bot->memory.capacity = 8;
    bot->memory.certainCards = malloc(bot->memory.capacity * sizeof(CertainCard));
}

void doTurn(GameState *game) {
    loadMemory(&game->stream, g_bot.memory);

    // if u have certain cards in hand, ask for them
    // else ask for the card you have either
    // 1. not asked for yet at paticular target
    // 2. been the longest since you asked for it
}