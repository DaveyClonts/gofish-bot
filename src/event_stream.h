#ifndef EVENT_STREAM_H
#define EVENT_STREAM_H
#include "deck.h"
#include "event_buffer.h"

// found
// not found

// CARD_REQUESTED
    // actorId
    // targeted card
// CARD_DRAWN
    // card drawn
// trade
    // taker
    // target
    // cards traded
// book found
    // owner
    // value of book

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
    int numOfCardsTrasnferred;
} Event;

typedef struct {
    Event *events;
    int size;
    int capacity;
    EventBuffer eventBuffer;
} EventStream;

#endif