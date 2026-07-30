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
bool publishEventStream(EventStream *stream, Event event) {
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
            checkEventBufferCapacity(&stream->eventBuffer);
            snprintf(stream->eventBuffer.buffer, sizeof(stream->eventBuffer.capacity), "Player %d's turn: ", event.actorId);
            break;
        case CARD_REQUESTED:
            checkEventBufferCapacity(&stream->eventBuffer);
            snprintf(stream->eventBuffer.buffer, sizeof(stream->eventBuffer.capacity), "Do you have any %ds?\n", event.targetId);
            break;
        case CARD_TRANSFERRED:
            checkEventBufferCapacity(&stream->eventBuffer);
            snprintf(stream->eventBuffer.buffer, sizeof(stream->eventBuffer.capacity), "Player %d transfered %d %d cards to player %d\n", event.actorId, event.numOfCardsTrasnferred, event.value, event.targetId);
            break;
        case GO_FISH:
            checkEventBufferCapacity(&stream->eventBuffer);
            snprintf(stream->eventBuffer.buffer, sizeof(stream->eventBuffer.capacity), "Go fish!\n");
            break;
        case CARD_DRAWN:
            checkEventBufferCapacity(&stream->eventBuffer);
            snprintf(stream->eventBuffer.buffer, sizeof(stream->eventBuffer.capacity), "Player %d draws a %d\n", event.actorId, event.value);
            break;
        case BOOK_FOUND:
            checkEventBufferCapacity(&stream->eventBuffer);
            snprintf(stream->eventBuffer.buffer, sizeof(stream->eventBuffer.capacity), "Book found by player %d for the %d card!\n", event.actorId, event.value);
            break;
        case GAME_WON:
            checkEventBufferCapacity(&stream->eventBuffer);
            snprintf(stream->eventBuffer.buffer, sizeof(stream->eventBuffer.capacity), "Game won by player %d!\n", event.actorId);
            break;
        default:
            fprintf(stderr, "Error: unrecognized eventType");
            return false;
    }


}