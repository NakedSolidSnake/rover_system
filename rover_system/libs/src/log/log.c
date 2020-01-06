#include <log/log.h>
#include <syslog.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>


int log(int logType, const char *module, const char *logMessage)
{
  (void)logType;

  openlog(module, LOG_PID | LOG_CONS , LOG_USER);
  syslog(LOG_INFO, logMessage);
  closelog();
  return EXIT_SUCCESS;
}

int logArgs(int logType, const char *module, const char *logMessage, ...)
{
  char buffer[MAX_BUF] = {0};
  va_list args;
  va_start(args, logMessage);
  vsprintf(buffer, logMessage, args);
  va_end(args);

  return log(LOG_INFO, module, buffer);
}
