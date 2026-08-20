#include "bot.h"
#include "event_stream.h"
#include "game.h"
#include "stack.h"
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
    }

    publishEvent(&game->stream,
        (Event){
            .eventType = GO_FISH,
        });

    if (!stackIsEmpty(&game->drawPile)) {
        drawCard(game, asker);
        checkPlayersForBook(game);
        if (checkForWin(game)) {
            tui_winScreen(game);
        }
    }

    tui_displayTurn(game);
    tui_waitForKey();
    return false;
}

void doTurn(BotState *bot, GameState *game) {
    Player *botPlayer = &game->players[bot->playerId];

    while (!game->winCondition.hasWon) {
        loadMemory(bot, &game->stream);

        if (botPlayer->handSize == 0) {
            publishEvent(&game->stream,
                (Event){
                    .eventType = EMPTY_HAND,
                    .actorId = bot->playerId,
                });

            if (stackIsEmpty(&game->drawPile)) {
                return;
            }

            drawCard(game, &game->players[bot->playerId]);
        }

        int targetId = 0;
        values requestedValue = botPlayer->hand[0].value;
        bool foundCertainCard = false;

        if (botPlayer->handSize > 0) {
            // If the bot has a matching card, request that
            for (int i = 0; i < bot->memory.size && !foundCertainCard; i++) {
                if (bot->memory.certainCards[i].targetId == bot->playerId) {
                    continue;
                }

                for (int j = 0; j < botPlayer->handSize; j++) {
                    if (bot->memory.certainCards[i].cardValue == botPlayer->hand[j].value) {
                        targetId = bot->memory.certainCards[i].targetId;
                        requestedValue = bot->memory.certainCards[i].cardValue;
                        foundCertainCard = true;
                        break;
                    }
                }
            }

            if (!foundCertainCard) {

                if (bot->fallbackMemory.fallbackTargetId == botPlayer->playerNum) {
                    bot->fallbackMemory.fallbackTargetId++;
                }

                if (bot->fallbackMemory.fallbackTargetId >= game->playerCount) {
                    bot->fallbackMemory.fallbackTargetId = 0;
                }

                if (bot->fallbackMemory.fallBackCardIndex >= botPlayer->handSize) {
                    bot->fallbackMemory.fallBackCardIndex = 0;
                }

                requestedValue = botPlayer->hand[bot->fallbackMemory.fallBackCardIndex].value;
                targetId = bot->fallbackMemory.fallbackTargetId;
                bot->fallbackMemory.fallbackTargetId++;

                if (bot->fallbackMemory.fallbackTargetId == botPlayer->playerNum) {
                    bot->fallbackMemory.fallbackTargetId++;
                }

                if (bot->fallbackMemory.fallbackTargetId >= game->playerCount) {
                    bot->fallbackMemory.fallbackTargetId = 0;

                    if (bot->fallbackMemory.fallbackTargetId == botPlayer->playerNum) {
                        bot->fallbackMemory.fallbackTargetId++;
                    }

                    bot->fallbackMemory.fallBackCardIndex++;
                }
            }

            if (!botAskForCard(game, botPlayer->playerNum, targetId, requestedValue)) {
                return;
            }
        }
    }
}
