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
    char* ptr = dest + strlen(dest);
 
    while (*src != '\0')
        *ptr++ = *src++;
 
    *ptr = '\0';
 
    return dest;
}

char *itoa(unsigned int num, int base) {
    static char representation[] = "0123456789ABCDEF";
    static char buffer[64];

    char *ptr;
    ptr = &buffer[64];
    *ptr = '\0';

    do {
        *--ptr = representation[num % base];
        num /= base;
    } while (num != 0);

    return (ptr);
}

void vsprintf(char *str, char *format, va_list arg) {
    char *traverse;
    unsigned int i;
    char *s;

    for (traverse = format; *traverse != '\0'; traverse++) {
        while (*traverse != '%') {
            char ch = *traverse;
            char *ptr = &ch;

            strcat(str, ptr);
            traverse++;
        }

        traverse++;

        switch (*traverse) {
            /*case 'c':
                i = va_arg(arg, int);
                
                strcat(str, i);

                break;*/

            case 'd':
                i = va_arg(arg, int);
                if (i < 0) {
                    i = -i;
                    
                    strcat(str, "-");
                }
                strcat(str, itoa(i, 10));
                break;

            case 'o':
                i = va_arg(arg, unsigned int);
                strcat(str, itoa(i, 8));
                break;

            case 's':
                s = va_arg(arg, char *);
                strcat(str, s);
                break;

            case 'x':
                i = va_arg(arg, unsigned int);
                strcat(str, itoa(i, 16));
                break;
        }
    }
}


// TODO: Improve this function and maybe move it
void sprintf(char *str, char *format, ...) {
    va_list args;
    va_start(args, format);

    vsprintf(str, format, args);

    va_end(args);
}