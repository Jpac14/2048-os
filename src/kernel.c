#include <stdint.h>
#include <stddef.h>
#include "stivale2.h"

#include <devices/serial/serial.h>
#include <devices/video/video.h>

static uint8_t stack[4096];

struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0
    },
    .framebuffer_width = 0,
    .framebuffer_height = 0
};

__attribute__((section(".stivale2hdr"), used))
struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = 0,
    .tags = (uintptr_t)&framebuffer_hdr_tag
};

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {
        if (current_tag == NULL) {
            return NULL;
        }

        if (current_tag->identifier == id) {
            return current_tag;
        }

        current_tag = (void *)current_tag->next;
    }
}

void _start(struct stivale2_struct *stivale2_struct) {
    struct stivale2_struct_tag_framebuffer *fb_hdr_tag;
    fb_hdr_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

    init_serial();
    puts("Hello");

    init_video(fb_hdr_tag);
    colour_t bg = {255, 255, 255};
    set_screen(get_colour(&bg));

    colour_t a = {255, 0, 0};
    set_pixel(1023, 767, get_colour(&a));


    for (;;) {
        asm ("hlt");
    }
}
