#include <stdint.h>
#include <stivale2.h>
#include <devices/serial/serial.h>
#include <libstr/string.h>

#ifndef VIDEO_H
#define VIDEO_H

typedef struct {
    uint8_t r, g, b;
} colour_t;

uint32_t get_colour(colour_t *colour);
void set_pixel(uint16_t x, uint16_t y, uint32_t colour);
void set_screen(uint32_t colour);
void init_video(struct stivale2_struct_tag_framebuffer *fb);

#endif