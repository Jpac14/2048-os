#include <inc/stivale2.h>
#include <devices/serial/serial.h>
#include <stdint.h>
#include <stddef.h>

#include "font.h"

#ifndef VIDEO_H
#define VIDEO_H

#define RED_SHIFT 16
#define GREEN_SHIFT 8
#define BLUE_SHIFT 0

typedef struct {
    uint8_t r, g, b;
} colour_t;

uint32_t get_colour(colour_t *colour);
void set_pixel(uint16_t x, uint16_t y, uint32_t colour);
void set_screen(uint32_t colour);
void video_putchar(char ch, int x, int y, colour_t colour);
void video_put(char c, colour_t colour);
void video_print(char *string, colour_t colour);
void video_printf(char *format, colour_t colour, ...);
void init_video(struct stivale2_struct_tag_framebuffer *fb);

#endif