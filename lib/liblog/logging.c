#include "logging.h"

void log(enum Status status, char *format, ...) {
  char *template;
  switch (status) {
    case INFO:
      template = "%d:%d:%d [\033[1;34m INFO \033[1;34m\033[0m] ";
      break;
    case DEBUG:
      template = "%d:%d:%d [\033[1;36m DEBUG \033[1;36m\033[0m] ";
      break;
    case WARNING:
      template = "%d:%d:%d [\033[1;33m WARNING \033[1;33m\033[0m] ";
      break;
    case ERROR:
      template = "%d:%d:%d [\033[1;31m ERROR \033[1;31m\033[0m] ";
      break;
    case PANIC:
      template = "%d:%d:%d [\033[1;31m PANIC \033[1;31m\033[0m] ";
      break;
  }

  char buffer[1000] = {0};
  va_list args;
  va_start(args, format);

  vsprintf(buffer, format, args);

  va_end(args);

  datetime_t current = rtc_get_datetime();

  serial_printf(template, current.hour, current.minute, current.second);
  serial_printf("%s\n", buffer);
}
