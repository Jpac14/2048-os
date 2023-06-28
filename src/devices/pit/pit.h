#ifndef PIT_H
#define PIT_H

#include <libasm/asm.h>
#include <liblog/logging.h>
#include <stdint.h>

void pit_add_ticks(void);
uint64_t pit_get_ticks(void);
void init_pit(uint16_t frequency);

#endif
