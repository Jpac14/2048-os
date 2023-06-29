#include "serial.h"

#define COM1 0x3F8

void init_serial(void) {
  outb(COM1 + 1, 0x00);  // Disable all interrupts
  outb(COM1 + 3, 0x80);  // Enable DLAB (set baud rate divisor)
  outb(COM1 + 0, 0x03);  // Set divisor to 3 (lo byte) 38400 baud
  outb(COM1 + 1, 0x00);  //                  (hi byte)
  outb(COM1 + 3, 0x03);  // 8 bits, no parity, one stop bit
  outb(COM1 + 2, 0xC7);  // Enable FIFO, clear them, with 14-byte threshold
  outb(COM1 + 4, 0x0B);  // IRQs enabled, RTS/DSR set

  log(INFO, "Serial initialized");
}

int serial_recieved(void) { return inb(COM1 + 5) & 1; }

char read_serial(void) {
  while (serial_recieved() == 0)
    ;

  return inb(COM1);
}

int serial_transmit_ready(void) { return inb(COM1 + 5) & 0x20; }

void serial_put(char ch) {
  while (serial_transmit_ready() == 0)
    ;

  outb(COM1, ch);
}

void serial_puts(char *string) {
  for (unsigned int i = 0; i < strlen(string); i++) {
    char ch = string[i];
    serial_put(ch);
  }
}

void serial_printf(char *format, ...) {
  char buffer[1000] = {0};

  va_list args;
  va_start(args, format);

  vsprintf(buffer, format, args);

  va_end(args);

  serial_puts(buffer);
}
