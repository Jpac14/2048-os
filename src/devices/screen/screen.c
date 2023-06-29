#include "screen.h"

// double buffers
uint32_t buffers[2][BUFFER_SIZE];
uint8_t back = 0;

#define CURRENT (buffers[back])
#define SWAP() (back = 1 - back)

struct stivale2_struct_tag_framebuffer *fb_info;

void screen_swap(void) {
  memcpy((void *)fb_info->framebuffer_addr, &CURRENT, 7680 * 800);
}

uint32_t get_colour(colour_t *colour) {
  return (uint32_t)((colour->r << RED_SHIFT) | (colour->g << GREEN_SHIFT) |
                    (colour->b << BLUE_SHIFT));
}

void set_pixel(uint16_t x, uint16_t y, uint32_t colour) {
  size_t pixel_offset = x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * y;

  CURRENT[pixel_offset] = colour;
}

void set_screen(uint32_t colour) { memset(&CURRENT, colour, BUFFER_SIZE); }

void init_screen(struct stivale2_struct *info) {
  struct stivale2_struct_tag_framebuffer *fb_hdr_tag;
  fb_hdr_tag = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

  if (fb_hdr_tag == NULL) {
    for (;;) {
      __asm__ volatile("hlt");
    }
  }

  fb_info = fb_hdr_tag;

  log(INFO, "Screen initialized");
  log(INFO, "Resolution: %dx%d", fb_info->framebuffer_width,
      fb_info->framebuffer_height);
  log(INFO, "Pitch: %d", fb_info->framebuffer_pitch);
  log(INFO, "BPP: %d", fb_info->framebuffer_bpp);
}
