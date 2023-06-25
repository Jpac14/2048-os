#ifndef VIDEO_H
#define VIDEO_H

#include <boot/stivale2.h>
#include <devices/serial/serial.h>
#include <liblog/logging.h>
#include <stddef.h>
#include <stdint.h>

#include "font.h"

enum ParsingState { OFF, PARSING };

#define RED_SHIFT 16
#define GREEN_SHIFT 8
#define BLUE_SHIFT 0

typedef struct {
  uint8_t r, g, b;
} colour_t;

struct text_rendering_info {
  colour_t color_scheme[8];
  colour_t background;
  colour_t text;
  size_t cursor_x;
  size_t cursor_y;
  enum ParsingState parsing_state;
};

colour_t rgb(int r, int g, int b);
uint32_t get_colour(colour_t *colour);
void set_pixel(uint16_t x, uint16_t y, uint32_t colour);
void set_screen(uint32_t colour);
void video_clear(void);
void video_putchar(char ch, int x, int y, colour_t colour);
void video_put(char c);
void video_puts(char *string);
void video_printf(char *format, ...);
void init_video(struct stivale2_struct *info);

#endif
