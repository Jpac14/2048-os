#ifndef SERIAL_H
#define SERIAL_H

#include <libasm/asm.h>
#include <liblog/logging.h>
#include <libstr/string.h>
#include <stdarg.h>
#include <stddef.h>

void init_serial(void);
int serial_recieved(void);
char read_serial(void);
int serial_transmit_ready(void);
void serial_put(char ch);
void serial_puts(char *string);
void serial_printf(char *format, ...);

#endif
