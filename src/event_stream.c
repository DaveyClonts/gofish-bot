#include "event_stream.h"
#include <stdio.h>
#include <stdbool.h>

void eventStreamInit(EventStream *eventStream) {
    eventStream->size = 0;
    eventStream->capacity = 16;
    eventStream->eventBuffer[0] = '\0';
}

bool publishEventStream(EventStream *eventStream, event_type eventType) {

    Event newEvent;

    switch (eventType) {
        case TURN_STARTED:
            newEvent.actorId = 
            newEvent.eventType = TURN_STARTED;
            break;
        case CARD_REQUESTED:
            
            break;
        case CARD_TRANSFERRED:

            break;
        case CARD_DRAWN:

            break;
        case GO_FISH:

            break;
        case BOOK_FOUND:

            break;
        case GAME_WON:

            break;
        default:
            fprintf(stderr, "Error: unrecognized eventType");
            return false;
    }


}