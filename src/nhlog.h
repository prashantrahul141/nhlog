#ifndef _NHLOG_H_
#define _NHLOG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Different log levels
 */
typedef enum : uint8_t {
  NHLOG_TRACE = 0,
  NHLOG_DEBUG,
  NHLOG_INFO,
  NHLOG_WARN,
  NHLOG_ERROR,
  NHLOG_FATAL,
  NHLOG_OFF = 255
} LogLevel;

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
  const size_t line;
  // level of the event
  const LogLevel level;
} LogEvent;

/*
 * Sets the logging level
 * default is NHLOG_TRACE
 * @param level - the level
 */
void nhlog_set_level(LogLevel level);

/*
 * Retrieve current logging level
 * @returns Current logging level
 */
LogLevel nhlog_get_level(void);

/*
 * Sets the logging out stream
 * default is stderr
 * @param fd - pointer to file stream, stderr will be used if given NULL.
 */
void nhlog_set_outstream(FILE *fd);

/*
 * Retrieve current logging output stream
 * @returns Current logging output stream
 */
FILE *nhlog_get_outstream(void);

/*
 * Sets whether to output colors or not
 * default is set to true
 * @param output_colors Bool
 */
void nhlog_set_output_colors(bool output_colors);

/*
 * Retrieve current output colors boolean
 * @returns Current out colors boolean
 */
bool nhlog_get_output_colors(void);

/*
 * Sets whether to flush output stream after every log or not
 * default is set to true
 * @param immediate boolean
 */
void nhlog_set_immediate(bool immediate);

/*
 * Retrieve immediate flag is set or not
 * @returns Current state of immediate flag
 */
bool nhlog_get_immediate(void);

void nhlog_log(LogLevel level, const char *file, size_t line, const char *fmt,
               ...);

#define TRACE(...) nhlog_log(NHLOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define DEBUG(...) nhlog_log(NHLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define INFO(...) nhlog_log(NHLOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define WARN(...) nhlog_log(NHLOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define ERROR(...) nhlog_log(NHLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define FATAL(...) nhlog_log(NHLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
