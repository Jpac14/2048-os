#include "pit.h"

volatile uint64_t ticks = 0;

void pit_add_ticks(void) {
  ticks++;
  outb(0x20, 0x20);
}

uint64_t pit_get_ticks(void) { return ticks; }

void init_pit(uint16_t frequency) {
  uint32_t divisor = 1193181 / frequency;

  outb(0x43, 0x36);
  outb(0x40, (uint8_t)divisor & 0xFF);
  outb(0x40, (uint8_t)(divisor >> 8) & 0xFF);

  log(INFO, "Initialized PIT with frequency: %d Hz", frequency);
}
