#include "event_log.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void eventLogInit(EventLog *log) {
    log->capacity = 64;
    log->length = 0;
    log->log = malloc(log->capacity);

    if (log->log == NULL) {
        fprintf(stderr, "Error: failed to allocate %zu bytes", log->capacity);
        exit(EXIT_FAILURE);
    }

    log->log[0] = '\0';
}

void writeToLog(EventLog *log, const char *message, ...) {

    // these act as cursors ig? meaning they get consumed?
    va_list arguments;
    va_list argumentsCopy;

    va_start(arguments, message);
    va_copy(argumentsCopy, arguments);

    //returns number of chars needed to complete formatted result needs
    int messageLength = vsnprintf(NULL, 0, message, argumentsCopy);
    if (messageLength < 0) {
        va_end(arguments);
        fprintf(stderr, "Error: vsnprintf failed");
        exit(EXIT_FAILURE);
    }
    va_end(argumentsCopy);

    size_t requiredCapacity = log->length + (size_t)messageLength + 1;

    if (requiredCapacity > log->capacity) {
        size_t newCapacity = log->capacity;

        newCapacity *= 2;
        char *newLog = realloc(log->log, newCapacity);

        if (newLog == NULL) {
            fprintf(stderr, "Error: failed to allocate new log");
            exit(EXIT_FAILURE);
        }

        log->log = newLog;
        log->capacity = newCapacity;
    }

    // write to log
    vsnprintf(log->log + log->length, log->capacity - log->length, message, arguments);

    va_end(arguments);

    log->length += (size_t)messageLength;
}