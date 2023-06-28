#ifndef PANIC_H
#define PANIC_H

#include <devices/serial/serial.h>

void __panic(char *file, const char function[20], int line, char *message);
#define panic(msg) __panic(__FILE_NAME__, __FUNCTION__, __LINE__, msg);

#endif
