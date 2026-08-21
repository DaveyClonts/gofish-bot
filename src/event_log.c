#include "event_log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void eventLogInit(EventLog *log) {
    log->capacity = 64;
    log->length = 0;
    log->log = malloc(log->capacity * sizeof(*log->log));

    if (log->log == NULL) {
        fprintf(stderr, "Error: failed to allocate %zu bytes", log->capacity);
        exit(EXIT_FAILURE);
    }
}

void writeToLog(EventLog *log, const char *message, ...) {

    // these act as cursors ig? meaning they get consumed?
    va_list arguments;
    va_list argumentsCopy;

    va_start(arguments, message);
    va_copy(argumentsCopy, arguments);

    // returns number of chars needed to complete formatted result needs
    int messageLength = vsnprintf(NULL, 0, message, argumentsCopy);

    if (messageLength < 0) {
        va_end(argumentsCopy);
        va_end(arguments);
        fprintf(stderr, "Error: vsnprintf failed");
        exit(EXIT_FAILURE);
    }
    va_end(argumentsCopy);

    if (log->length == log->capacity) {
        size_t newCapcity = log->capacity * 2;
        char **newLog = realloc(log->log, newCapcity * sizeof(*newLog));

        if (newLog == NULL) {
            va_end(arguments);
            fprintf(stderr, "Error: new log realloc failed");
            exit(EXIT_FAILURE);
        }

        log->log = newLog;
        log->capacity = newCapcity;
    }

    size_t castedMessageSize = (size_t)messageLength + 1;
    char *entry = malloc(castedMessageSize); //+1 for \0 character

    if (entry == NULL) {
        va_end(arguments);
        fprintf(stderr, "Error: failed to allocate log entry");
        exit(EXIT_FAILURE);
    }

    // write to log
    vsnprintf(entry, castedMessageSize, message, arguments);
    va_end(arguments);

    log->log[log->length] = entry;
    log->length++;
}