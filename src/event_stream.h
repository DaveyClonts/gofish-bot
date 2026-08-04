#ifndef EVENT_STREAM_H
#define EVENT_STREAM_H
#include "deck.h"
#include "event_log.h"

typedef enum {
    TURN_STARTED,
    CARD_REQUESTED,
    CARD_TRANSFERRED,
    GO_FISH,
    EMPTY_HAND,
    CARD_DRAWN,
    BOOK_FOUND,
    GAME_WON
} event_type;

typedef struct {
    event_type eventType;
    int actorId;
    int targetId;
    values value;
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
