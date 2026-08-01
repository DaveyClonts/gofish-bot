#ifndef EVENT_STREAM_H
#define EVENT_STREAM_H
#include "deck.h"
#include "event_log.h"

// found
// not found

// CARD_REQUESTED
    // actorId
    // targeted card
// CARD_DRAWN
    // card drawn
// CARD_TRANSFERRED
    // actorId
    // targetId
    // numOfCardsTransferred
    // Value of card transferred
// BOOK_FOUND
    // actorId
    // value of book
// GAME_WON
    // actorId - in this case winnerId

typedef enum { 
    TURN_STARTED,
    CARD_REQUESTED,
    CARD_TRANSFERRED,
    GO_FISH,
    CARD_DRAWN,
    BOOK_FOUND,
    GAME_WON
} event_type;

typedef struct {
    event_type eventType;
    int actorId;
    int targetId;
    values value;
    int numOfCardsTransferred;
} Event;

typedef struct {
    Event *events;
    int size;
    int capacity;
    EventLog eventLog;
} EventStream;

void eventStreamInit(EventStream *stream);

void publishEvent(EventStream *stream, Event event);

#endif
