#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>
#include <system/panic/panic.h>
void exception_handler(uint64_t irq);

#endif
