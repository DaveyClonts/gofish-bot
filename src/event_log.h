#ifndef EVENT_LOG_H
#define EVENT_LOG_H
#include <stddef.h>

typedef struct {
    char **log;
    size_t capacity;
    size_t length;
} EventLog;

void eventLogInit(EventLog *log);

void writeToLog(EventLog *log, const char *message, ...);

#endif
