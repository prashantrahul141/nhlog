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

static struct {
  // current logging level
  int level;
  // file stream to write to
  FILE *fd;
} logger_state;

static void nhlog_stdout(LogEvent *event) {
  char time_buffer[16];
  time_buffer[strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S",
                       event->time)] = '\0';

  bool is_file = !(stdout == event->udata || stderr == event->udata);
  // if its not a file output with colors
  if (!is_file) {
    fprintf(event->udata, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m %s",
            time_buffer, level_colors[event->level],
            level_strings[event->level], event->file, event->line,
            level_colors[event->level]);
  } else {
    fprintf(event->udata, "%s %-5s %s:%d: ", time_buffer,
            level_strings[event->level], event->file, event->line);
  }

  vfprintf(event->udata, event->fmt, event->ap);

  if (!is_file) {
    // if its not a file output with colors
    fprintf(event->udata, "\n\x1b[0m");
  } else {
    fprintf(event->udata, "\n");
  }

  fflush(event->udata);
}

void nhlog_init(LogLevel level, FILE *outstream) {
  logger_state.level = level;
  logger_state.fd = NULL == outstream ? stderr : outstream;
}

void nhlog_set_level(LogLevel level) { logger_state.level = level; }

void nhlog_set_outstream(FILE *fd) {
  logger_state.fd = NULL == fd ? stderr : fd;
}

void nhlog_log(LogLevel level, const char *file, int line, const char *fmt,
               ...) {
  time_t t = time(NULL);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
  LogEvent event = {
      .fmt = fmt,
      .file = file,
      .time = localtime(&t),
      .udata = logger_state.fd,
      .line = line,
      .level = level,
  };
#pragma GCC diagnostic pop

  if (level >= logger_state.level) {
    va_start(event.ap, fmt);
    nhlog_stdout(&event);
    va_end(event.ap);
  }
}

#ifdef __cplusplus
} // extern "C"
#endif
