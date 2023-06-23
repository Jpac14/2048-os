#include "keyboard.h"

unsigned char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\0',
    '\t',
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0,
    '*',
    0,
    ' ',
    0,
    0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0,
    0,
    0,
    '-',
    0,
    0,
    0,
    '+',
    0,
    0,
    0,
    0,
    0,
    0, 0, 0,
    0,
    0,
    0};


void keyboard_main() {
    uint8_t state = inb(0x64);

    while (state & 1) {
        uint8_t keycode = inb(0x60);
        uint8_t scan_code = keycode & 0x7f;
        uint8_t key_state = !(keycode & 0x80);
        colour_t text = {255, 255, 255};

        if (key_state) {
            video_put(keyboard_map[(unsigned char)scan_code], text);
        }

        state = inb(0x64);
    }

    outb(0x20, 0x20);
}