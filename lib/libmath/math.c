#include "math.h"

int pow(int base, int exponent) {
  int i;
  int result = 1;

  for (i = 0; i < exponent; i++) {
    result *= base;
  }

  return result;
}
