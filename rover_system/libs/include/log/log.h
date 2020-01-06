#ifndef __LOG_H
#define __LOG_H

#define MAX_BUF         1024

#define LOG_INFO        0
#define LOG_WARNING     1
#define LOG_CRITICAL    2
#define LOG_FATAL       3

int log(int logType, const char *module, const char *logMessage);
int logArgs(int logType, const char *module, const char *logMessage, ...);

#endif
