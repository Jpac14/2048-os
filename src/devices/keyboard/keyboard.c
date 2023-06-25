#include "keyboard.h"

void keyboard_main(void) {
  uint8_t state = inb(0x64);

  while (state & 1) {
    uint8_t keycode = inb(0x60);
    uint8_t scan_code = keycode & 0x7f;
    uint8_t key_state = !(keycode & 0x80);

    if (key_state) {
      video_put(keyboard_map[scan_code]);
    }

    state = inb(0x64);
  }

  outb(0x20, 0x20);
}

void init_keyboard(void) { log(INFO, "Keyboard initalized"); }
