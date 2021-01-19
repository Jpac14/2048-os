#include "video.h"

struct stivale2_struct_tag_framebuffer *fb_info;

uint32_t get_colour(colour_t *colour) {
    return (uint32_t)((colour->r << 16) | (colour->g << 8) | 
        (colour->b << 0));
}

void set_pixel(uint16_t x, uint16_t y, uint32_t colour) {
    uint32_t *fb_addr = (uint32_t *) fb_info->framebuffer_addr;
    uint16_t pixel_offset = x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * y;

    fb_addr[pixel_offset] = colour;
}

void set_screen(uint32_t colour) {
    for (uint16_t x = 0; x < fb_info->framebuffer_width; x++) {
        for (uint16_t y = 0; y < fb_info->framebuffer_height; y++) {
            set_pixel(x, y, colour);
        }
    }
}

void init_video(struct stivale2_struct_tag_framebuffer *fb) {
    fb_info = fb;
}

