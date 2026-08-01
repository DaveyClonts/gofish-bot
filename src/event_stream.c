#include "event_stream.h"
#include "event_buffer.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void eventStreamInit(EventStream *stream) {
    stream->size = 0;
    stream->capacity = 16;
    eventBufferInit(&stream->eventBuffer);

    stream->events = malloc(stream->capacity * sizeof(Event));
}

// make event with compund literal
bool publishEvent(EventStream *stream, Event event) {
    if(stream->size >= stream->capacity) {
        stream->capacity *= 2;
        Event *reallocEvents = realloc(stream->events, stream->capacity * sizeof(Event));
        
        if (reallocEvents == NULL) {
            fprintf(stderr, "Error: event stream realloc failed");
            exit(EXIT_FAILURE);
        }

        stream->events = reallocEvents;
    }

    switch (event.eventType) {
        case TURN_STARTED:
            writeToBuffer(&stream->eventBuffer, "Player %d's turn: ", event.actorId);
            break;
        case CARD_REQUESTED:
            writeToBuffer(&stream->eventBuffer, "Do you have any %ds?\n", event.targetId);
            break;
        case CARD_TRANSFERRED:
            writeToBuffer(&stream->eventBuffer, "Player %d transfered %d %d cards to player %d\n", event.actorId, event.numOfCardsTrasnferred, event.value, event.targetId);
            break;
        case GO_FISH:
            writeToBuffer(&stream->eventBuffer, "Go fish!\n");
            break;
        case CARD_DRAWN:
            writeToBuffer(&stream->eventBuffer, "Player %d draws a %d\n", event.actorId, event.value);
            break;
        case BOOK_FOUND:
            writeToBuffer(&stream->eventBuffer, "Book found by player %d for the %d card!\n", event.actorId, event.value);
            break;
        case GAME_WON:
            writeToBuffer(&stream->eventBuffer, "Game won by player %d!\n", event.actorId);
            break;
        default:
            fprintf(stderr, "Error: unrecognized eventType");
            exit(EXIT_FAILURE);
            return false;
    }


}