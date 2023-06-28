#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <libasm/asm.h>
#include <liblog/logging.h>
#include <stdbool.h>
#include <stdint.h>

#define KEY_NULL 0
#define KEY_ESC 27
#define KEY_BACKSPACE '\b'
#define KEY_TAB '\t'
#define KEY_ENTER '\n'
#define KEY_RETURN '\r'

#define KEY_INSERT 0x90
#define KEY_DELETE 0x91
#define KEY_HOME 0x92
#define KEY_END 0x93
#define KEY_PAGE_UP 0x94
#define KEY_PAGE_DOWN 0x95
#define KEY_LEFT 0x4B
#define KEY_UP 0x48
#define KEY_RIGHT 0x4D
#define KEY_DOWN 0x50

#define KEY_F1 0x80
#define KEY_F2 (KEY_F1 + 1)
#define KEY_F3 (KEY_F1 + 2)
#define KEY_F4 (KEY_F1 + 3)
#define KEY_F5 (KEY_F1 + 4)
#define KEY_F6 (KEY_F1 + 5)
#define KEY_F7 (KEY_F1 + 6)
#define KEY_F8 (KEY_F1 + 7)
#define KEY_F9 (KEY_F1 + 8)
#define KEY_F10 (KEY_F1 + 9)
#define KEY_F11 (KEY_F1 + 10)
#define KEY_F12 (KEY_F1 + 11)

struct keyboard {
  bool keys[128];
  bool chars[128];
};

// clang-format off
static unsigned char keyboard_map[128] = {
  KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
  '-', '=', KEY_BACKSPACE, KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u',
  'i', 'o', 'p', '[', ']', KEY_ENTER, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
  'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
  ',', '.', '/', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
  KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
  KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
  KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
};
// clang-format on

void keyboard_main(void);
bool keyboard_key(uint8_t scan_code);
bool keyboard_char(unsigned char c);
void init_keyboard(void);

#endif
