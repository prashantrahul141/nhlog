#ifndef _NHLOG_H_
#define _NHLOG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdarg.h>
#include <stdio.h>

/*
 * Internal representation of a log event.
 */
typedef struct {
  // additional parameters
  va_list ap;
  // format string
  const char *fmt;
  // file in which it originated
  const char *file;
  // time of the event
  struct tm *time;
  // the stream to write to
  FILE *udata;
  // at which line number it originated
  int line;
  // level of the event
  int level;
} LogEvent;

/*
 * Different log levels
 */
typedef enum {
  NHLOG_TRACE = 0,
  NHLOG_DEBUG,
  NHLOG_INFO,
  NHLOG_WARN,
  NHLOG_ERROR,
  NHLOG_FATAL,
  NHLOG_OFF
} LogLevel;

/*
 * Initializes the logger, should be called ATLEAST ONCE from anywhere before
 * start logging.
 * @param level - logging level
 * @param outstream - where to write to, stderr will be used if given NULL.
 */
void nhlog_init(LogLevel level, FILE *outstream);

/*
 * Sets the logging level
 * @param level - the level
 */
void nhlog_set_level(LogLevel level);

/*
 * Sets the logging out stream
 * @param fd - pointer to file stream, stderr will be used if given NULL.
 */
void nhlog_set_outstream(FILE *fd);

void nhlog_log(LogLevel level, const char *file, int line, const char *fmt,
               ...);

#define nhlog_trace(...) nhlog_log(NHLOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define nhlog_debug(...) nhlog_log(NHLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define nhlog_info(...) nhlog_log(NHLOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define nhlog_warn(...) nhlog_log(NHLOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define nhlog_error(...) nhlog_log(NHLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define nhlog_fatal(...) nhlog_log(NHLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
