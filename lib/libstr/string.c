#include "string.h"

unsigned int strlen(const char *string) {
  unsigned int count = 0;

  while (*string != '\0') {
    count++;
    string++;
  }

  return count;
}

char *strcat(char *dest, const char *src) {
  char *ptr = dest + strlen(dest);

  while (*src != '\0') *ptr++ = *src++;

  *ptr = '\0';

  return dest;
}

char *itoa(unsigned int num, int base) {
  static char representation[] = "0123456789ABCDEF";
  static char buffer[50];

  char *ptr;
  ptr = &buffer[49];
  *ptr = '\0';

  do {
    *--ptr = representation[num % base];
    num /= base;
  } while (num != 0);

  return (ptr);
}

void vsprintf(char *str, char *format, va_list arg) {
  unsigned int i;
  unsigned int ZERO = 0;
  char *s;

  int position = 0;

  while (*format) {
    if (*format == '%') {
      format++;
      switch (*format) {
        case 'c':
          i = va_arg(arg, int);
          str[position] = i;
          position++;
          break;

        case 'd':
          i = va_arg(arg, int);
          if (i < ZERO) {
            i = -i;
            str[position] = '-';
          }
          strcat(str, itoa(i, 10));
          position += strlen(itoa(i, 10));

          break;

        case 'o':
          i = va_arg(arg, unsigned int);
          strcat(str, itoa(i, 8));
          position += strlen(itoa(i, 8));
          break;

        case 's':
          s = va_arg(arg, char *);
          strcat(str, s);
          position += strlen(s);
          break;

        case 'x':
          i = va_arg(arg, unsigned int);
          strcat(str, itoa(i, 16));
          position += strlen(itoa(i, 16));
          break;

        default:
          str[position] = '%';
          position++;
          break;
      }
    }

    else {
      str[position] = *format;
      position++;
    }

    format++;
  }
}
