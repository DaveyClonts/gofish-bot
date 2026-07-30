#ifndef EVENT_STREAM_H
#define EVENT_STREAM_H
#include "deck.h"

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
    CARD_DRAWN,
    GO_FISH,
    BOOK_FOUND,
    GAME_WON
} event_type;

typedef struct {
    event_type eventType;
    int actorId;
    int targetId;
    values value;
    int cardsTransfered;
} Event;

typedef struct {
    Event *events;
    int size;
    int capacity;
    char *eventBuffer;
} EventStream;

#endif