#include "event_stream.h"
#include "event_log.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void logEvent(EventStream *stream, Event event) {
    switch (event.eventType) {
    case TURN_STARTED:
        writeToLog(&stream->eventLog, "Player %d's turn: \n", (event.actorId + 1));
        break;
    case CARD_REQUESTED:
        writeToLog(&stream->eventLog, "Do you have any %ds?\n", event.value);
        break;
    case CARD_TRANSFERRED:
        writeToLog(&stream->eventLog,
            "Player %d transferred a %d card to player %d\n",
            event.actorId,
            event.value,
            event.targetId);
        break;
    case GO_FISH:
        writeToLog(&stream->eventLog, "Go fish!\n");
        break;
    case EMPTY_HAND:
        writeToLog(&stream->eventLog, "Player %d has an empty hand\n", event.actorId);
        break;
    case CARD_DRAWN:
        writeToLog(&stream->eventLog, "Player %d draws a %d\n", event.actorId, event.value);
        break;
    case BOOK_FOUND:
        writeToLog(&stream->eventLog,
            "Book found by player %d for the %d card!\n",
            event.actorId,
            event.value);
        break;
    case GAME_WON:
        writeToLog(&stream->eventLog, "Game won by player %d!\n", event.actorId);
        break;
    default:
        fprintf(stderr, "Error: unrecognized eventType\n");
        exit(EXIT_FAILURE);
    }
}

void eventStreamInit(EventStream *stream) {
    stream->size = 0;
    stream->capacity = 16;
    eventLogInit(&stream->eventLog);

    stream->events = malloc(stream->capacity * sizeof(Event));
}

// make event with compund literal
void publishEvent(EventStream *stream, Event event) {
    if (stream->size >= stream->capacity) {
        stream->capacity *= 2;
        Event *reallocEvents = realloc(stream->events, stream->capacity * sizeof(Event));

        if (reallocEvents == NULL) {
            fprintf(stderr, "Error: event stream realloc failed");
            exit(EXIT_FAILURE);
        }

        stream->events = reallocEvents;
    }

    stream->events[stream->size] = event;
    stream->size++;

    logEvent(stream, event);
}
