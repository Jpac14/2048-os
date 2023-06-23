#include <stdarg.h>

#ifndef STRING_H
#define STRING_H

unsigned int strlen(const char *string);
char *strcat(char *dest, const char *src);
char *itoa(unsigned int num, int base);
void vsprintf(char *str, char *format, va_list args);
void sprintf(char *str, char *format, ...);

#endif