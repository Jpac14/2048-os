#include <stdint.h>

#ifndef ASM_H
#define ASM_H

unsigned char inb(unsigned short port);
void outb(uint16_t port, uint8_t value);

#endif