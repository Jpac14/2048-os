#include "video.h"

struct stivale2_struct_tag_framebuffer *fb_info;

size_t cursor_x = 5;
size_t cursor_y = 5;

uint32_t get_colour(colour_t *colour) {
    return (uint32_t)((colour->r << RED_SHIFT) | (colour->g << GREEN_SHIFT) | 
        (colour->b << BLUE_SHIFT));
}

void set_pixel(uint16_t x, uint16_t y, uint32_t colour) {
    uint32_t *fb_addr = (uint32_t *) fb_info->framebuffer_addr;
    size_t pixel_offset = x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * y;

    fb_addr[pixel_offset] = colour;
}

void set_screen(uint32_t colour) {
    for (uint16_t x = 0; x < fb_info->framebuffer_width; x++) {
        for (uint16_t y = 0; y < fb_info->framebuffer_height; y++) {
            set_pixel(x, y, colour);
        }
    }
}

void video_putchar(char ch, int x, int y, colour_t colour) {
    for (uint8_t iy = 0; iy < 8; iy++) {
        for (uint8_t ix = 0; ix < 8; ix++) {
            if ((font[(uint8_t) ch][iy] >> ix) & 1) {
                uint64_t offset = ((iy + y) * fb_info->framebuffer_pitch) + ((ix + x) * 4);
                *(uint32_t *)((uint64_t)fb_info->framebuffer_addr + offset) = get_colour(&colour);
            }
        }
    }
}

void video_put(char c, colour_t colour) {
    if (c == '\n') {
        cursor_x = 5;
        cursor_y += 20;
    }

    if (c == '\0') {
        cursor_x -= 1;
        video_putchar(' ', cursor_x, cursor_y, colour);
        cursor_x++;
    } else {
        video_putchar(c, cursor_x, cursor_y, colour);

        cursor_x += 8;

        if (cursor_x >= (size_t) fb_info->framebuffer_width - 5) {
            cursor_x = 5;
            cursor_y += 10;
        }
    }
}

void video_print(char *string, colour_t colour) {
    while (*string) {
        video_put(*string++, colour);
    }
}

void video_printf(char *format, colour_t colour, ...) {
    char buffer[1000];
     
    va_list args;
    va_start(args, colour);

    vsprintf(buffer, format, args);

    va_end(args);

    video_print(buffer, colour);
}

void init_video(struct stivale2_struct_tag_framebuffer *fb) {
    fb_info = fb;
}

