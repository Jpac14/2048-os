#include "asm.h"

unsigned char inb(unsigned short port) {
  unsigned char ret;

  __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));

  return ret;
}

void outb(uint16_t port, uint8_t value) {
  __asm__ volatile("outb %0, %1" ::"a"(value), "Nd"(port));
}

void iowait(void) { outb(0x80, 0); }
