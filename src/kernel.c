#include "kernel.h"

void _start(struct stivale2_struct *info) {
  init_video(info);

  init_gdt();
  init_idt();

  init_serial();
  serial_puts("Hello From OS!\n");

  init_keyboard();

  video_puts("Hello From OS!\n");

  for (;;) {
    __asm__ volatile("hlt");
  }
}
