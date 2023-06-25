#include "logging.h"

void log(enum Status status, char *format, ...) {
  char *template;
  switch (status) {
    case INFO:
      template = "\0337[\0334INFO\0337] ";
      break;
    case DEBUG:
      template = "\0337[\0335DEBUG\0337] ";
      break;
    case WARNING:
      template = "\0337[\0333WARNING\0337] ";
      break;
    case ERROR:
      template = "\0337[\0331ERROR\0337] ";
      break;
    case PANIC:
      template = "\0337[\0331PANIC\0337] ";
      break;
  }

  char buffer[1000] = {0};
  va_list args;
  va_start(args, format);

  vsprintf(buffer, format, args);

  va_end(args);

  video_printf("%s %s\n", template, buffer);
}
