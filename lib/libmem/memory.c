#include "memory.h"

// TODO: Implement memcpy, memmove and memcmp

void *memset(void *bufptr, int value, size_t size) {
  unsigned char *buf = (unsigned char *)bufptr;
  size_t i;
  for (i = 0; i < size; i++) buf[i] = (unsigned char)value;

  return bufptr;
}
