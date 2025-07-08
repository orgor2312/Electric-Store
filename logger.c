#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"
#include <string.h>

#define LOG_FILE "log.txt"

void logAction(const char *action, const char *details)
{
    FILE *file = fopen(LOG_FILE, "a");
    if (!file)
    {
        perror("Error opening log file");
        return;
    }

    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = '\0'; 

    fprintf(file, "[%s] %s: %s\n", timestamp, action, details);
    fclose(file);
}

void printAndLog(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    FILE *file = fopen(LOG_FILE, "a");
    if (file)
    {
        fprintf(file, "[LOG] ");
        vfprintf(file, format, args);
        fclose(file);
    }
    else
    {
        perror("Error opening log file");
    }

    va_end(args);
}
