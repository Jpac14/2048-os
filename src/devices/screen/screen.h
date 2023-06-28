#ifndef SCREEN_H
#define SCREEN_H

#include <boot/stivale2.h>
#include <liblog/logging.h>
#include <libmem/memory.h>
#include <stddef.h>
#include <stdint.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define SCREEN_BPP 32
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)
#define BUFFER_SIZE SCREEN_SIZE *(SCREEN_BPP / sizeof(uint32_t))

#define RED_SHIFT 16
#define GREEN_SHIFT 8
#define BLUE_SHIFT 0

#define RGB(r, g, b) ((colour_t){r, g, b})

typedef struct {
  uint8_t r, g, b;
} colour_t;

void screen_swap(void);
uint32_t get_colour(colour_t *colour);
void set_pixel(uint16_t x, uint16_t y, uint32_t colour);
void set_screen(uint32_t colour);
void init_screen(struct stivale2_struct *info);

#endif
