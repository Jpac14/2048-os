#include <libasm/asm.h>
#include <libstr/string.h>
#include <stdarg.h>

#ifndef SERIAL_H
#define SERIAL_H

void init_serial();
int serial_recieved();
char read_serial();
int serial_transmit_ready();
void write_serial(char ch);
void serial_puts(char *string);
void serial_put(char ch);
void serial_printf(char *format, ...);

#endif