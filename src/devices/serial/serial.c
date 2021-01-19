#include "serial.h"

#include <libasm/asm.h>
#include <libstr/string.h>

#define COM1 0x3F8

void init_serial() {
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
}

int serial_recieved() {
    return inb(COM1 + 5) & 1;
}

char read_serial() {
    while (serial_recieved() == 0);

    return inb(COM1);
}

int serial_transmit_ready() {
    return inb(COM1 + 5) & 0x20;
}

void write_serial(char ch) {
    while (serial_transmit_ready() == 0);

    outb(COM1, ch);
}

void puts(char *string) {
    for (unsigned int i = 0; i < strlen(string); i++) {
        char ch = string[i];
        write_serial((int) ch);
    }
}

void put(char ch) {
    write_serial((int) ch);
}