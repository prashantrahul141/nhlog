#ifndef _NHLOG_H_
#define _NHLOG_H_

#include <stdarg.h>

typedef struct {
  va_list ap;
  const char *fmt;
  const char *file;
  struct tm *time;
  void *udata;
  int line;
  int level;
} LogEvent;

enum LogLevel {
  LOG_TRACE,
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL
};

void nhlog_set_level(int level);

void nhlog_log(int level, const char *file, int line, const char *fmt, ...);

#define log_trace(...) nhlog_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) nhlog_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) nhlog_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) nhlog_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) nhlog_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) nhlog_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif