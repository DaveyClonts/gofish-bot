#ifndef EVENT_LOG_H
#define EVENT_LOG_H

typedef struct {
    char *log;
    int capacity;
} EventLog;

void eventLogInit(EventLog *log);

void writeToLog(EventLog *log, const char *message, ...);

#endif
