#include "panic.h"

void __panic(char *file, const char function[20], int line, char *message) {
  serial_puts(
      "\033[31m----------------------------------------------------------------"
      "--------\n");
  serial_puts("KERNEL PANIC\n\033[0m");
  serial_printf("\033[35m%s\033[0m\n", message);
  serial_printf("In %s at %s(), line %d\n", file, function, line);

  for (;;) {
    __asm__ volatile("hlt");
  }
}
