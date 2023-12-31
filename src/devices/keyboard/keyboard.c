#include "keyboard.h"

struct keyboard keyboard;

void keyboard_main(void) {
  uint8_t state = inb(0x64);
  while (state & 1) {
    uint8_t keycode = inb(0x60);
    uint8_t scan_code = keycode & 0x7f;
    uint8_t key_state = !(keycode & 0x80);

    keyboard.keys[scan_code] = key_state;
    keyboard.chars[keyboard_map[scan_code]] = key_state;

    state = inb(0x64);
  }

  outb(0x20, 0x20);
}

bool keyboard_key(uint8_t scan_code) { return keyboard.keys[scan_code]; }
bool keyboard_char(unsigned char c) { return keyboard.chars[c]; }

void init_keyboard(void) { log(INFO, "Keyboard initialized"); }
