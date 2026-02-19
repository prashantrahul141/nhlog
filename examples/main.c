#include "../src/nhlog.h"

int main() {
  /* Zero initialisation needed */
  TRACE("least important messages");
  DEBUG("debug messages");
  INFO("general info message");
  WARN("warnings");
  ERROR("errors");
  FATAL("unrecoverable crashes");

  /* Logging level can be changed at runtime using: */
  nhlog_set_level(NHLOG_INFO);
  TRACE("this will be NOT be logged");
  WARN("however this will be logged");

  /* All logging can be disabled by setting log level to NHLOG_OFF */
  nhlog_set_level(NHLOG_OFF);
  TRACE("this will be NOT be logged");

  nhlog_set_level(NHLOG_TRACE);

  /* you can log to any stream by passing it to nhlog_set_outstream */
  FILE *fd = fopen("logs.txt", "w");
  nhlog_set_outstream(fd);

  /* you may also want to disable color output for files*/
  nhlog_set_output_colors(false);

  TRACE("these messages will now be logged to the file.");
  DEBUG("debug message");
  fclose(fd);

  /* setting to null defaults to stderr */
  nhlog_set_output_colors(true);
  nhlog_set_outstream(NULL);

  /* by default output stream if flushed after every log event, but it can be
   * disabled */
  nhlog_set_immediate(false);

  TRACE("These messages will be now buffered by the c runtime");

  return 0;
}
