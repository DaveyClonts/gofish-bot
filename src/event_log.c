#include "event_log.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void eventLogInit(EventLog *log) {
    log->capacity = 32;
    log->log = malloc(log->capacity * sizeof(char));
}

void writeToLog(EventLog *log, const char *message, ...) {
    char *reallocPtr = realloc(log->log, log->capacity * sizeof(message));
    if (reallocPtr == NULL) {
        fprintf(stderr, "Error: event log realloc failed");
        exit(EXIT_FAILURE);
    }
    log->log = reallocPtr;

    va_list arguments;
    va_start(arguments, message);
    vsnprintf(log->log, sizeof(message), message, arguments);
    va_end(arguments);
}
