#include "asm.h"

unsigned char inb(unsigned short port) {
    unsigned char ret;

    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));

    return ret;
}

void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" :: "a"(value), "Nd"(port));
}