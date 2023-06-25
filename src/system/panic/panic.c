#include "panic.h"

void __panic(char *file, const char function[20], int line, char *message) {
  video_clear();
  video_puts(
      "\0331-------------------------------------------------------------------"
      "-----\n");
  video_puts("\0331KERNEL PANIC\n");
  video_printf("\0335%s\n", message);
  video_printf("In %s, at %s(), line %d\n", file, function, line);

  for (;;) {
    __asm__ volatile("hlt");
  }
}
