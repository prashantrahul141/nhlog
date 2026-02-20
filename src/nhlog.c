#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "nhlog.h"

static const char *level_strings[] = {"TRACE", "DEBUG", "INFO",
                                      "WARN",  "ERROR", "FATAL"};

static const char *level_colors[] = {"\x1b[94m", "\x1b[36m", "\x1b[32m",
                                     "\x1b[33m", "\x1b[31m", "\x1b[35m"};

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

/*
 * Global internal state of the logger
 */
static struct {
  /* current logging level */
  LogLevel level;
  /* file stream to write to */
  FILE *fd;
  /* whether to output colors or not */
  bool output_colors;
  /* whether to flush output stream after every log event */
  bool immediate;
} logger_state = {
    .level = NHLOG_TRACE, .fd = NULL, .output_colors = true, .immediate = true};

/*
 * This writes a log event.
 */
static void nhlog_write(LogEvent *event) {
  char time_buffer[16];
  time_buffer[strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S",
                       event->time)] = '\0';

  /* if output_colors is set, output with colors */
  if (logger_state.output_colors) {
    fprintf(event->udata, "%s %s%-5s\x1b[0m \x1b[90m%s:%zu:\x1b[0m %s",
            time_buffer, level_colors[event->level],
            level_strings[event->level], event->file, event->line,
            level_colors[event->level]);
  } else {
    fprintf(event->udata, "%s %-5s %s:%zu: ", time_buffer,
            level_strings[event->level], event->file, event->line);
  }

  /* log message */
  vfprintf(event->udata, event->fmt, event->ap);

  /* if output_colors is set, output with colors */
  if (logger_state.output_colors) {
    fprintf(event->udata, "\n\x1b[0m");
  } else {
    fprintf(event->udata, "\n");
  }

  /* flush output stream if immediate flag is set  */
  if (logger_state.immediate) {
    fflush(event->udata);
  }
}

/* Getter and setter for level */
void nhlog_set_level(const LogLevel level) { logger_state.level = level; }
LogLevel nhlog_get_level(void) { return logger_state.level; }

/* Getter and setter for output stream */
void nhlog_set_outstream(FILE *fd) {
  logger_state.fd = NULL == fd ? stderr : fd;
}
FILE *nhlog_get_outstream(void) { return logger_state.fd; }

/* Getter and setter for output colors */
void nhlog_set_output_colors(const bool oc) { logger_state.output_colors = oc; }
bool nhlog_get_output_colors(void) { return logger_state.output_colors; }

/* Getter and setter for immediate */
void nhlog_set_immediate(const bool i) { logger_state.immediate = i; }
bool nhlog_get_immediate(void) { return logger_state.immediate; }

void nhlog_log(LogLevel level, const char *file, const size_t line,
               const char *fmt, ...) {
  const time_t t = time(NULL);
  LogEvent event = {
      .ap = {0},
      .fmt = fmt,
      .file = file,
      .time = localtime(&t),
      .udata = logger_state.fd == NULL ? stderr : logger_state.fd,
      .line = line,
      .level = level,
  };

  if (level >= logger_state.level) {
    va_start(event.ap, fmt);
    nhlog_write(&event);
    va_end(event.ap);
  }
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
