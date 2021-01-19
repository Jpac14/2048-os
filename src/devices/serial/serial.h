#ifndef SERIAL_H
#define SERIAL_H

void init_serial();
int serial_recieved();
char read_serial();
int serial_transmit_ready();
void write_serial(char ch);
void puts(char *string);
void put(char ch);

#endif