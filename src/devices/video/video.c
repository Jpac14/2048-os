#include "video.h"

struct stivale2_struct_tag_framebuffer *fb_info;
struct text_rendering_info text_info;

colour_t rgb(int r, int g, int b) { return (colour_t){r, g, b}; }

uint32_t get_colour(colour_t *colour) {
  return (uint32_t)((colour->r << RED_SHIFT) | (colour->g << GREEN_SHIFT) |
                    (colour->b << BLUE_SHIFT));
}

void set_pixel(uint16_t x, uint16_t y, uint32_t colour) {
  uint32_t *fb_addr = (uint32_t *)fb_info->framebuffer_addr;
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

void video_clear(void) {
  text_info.cursor_x = 5;
  text_info.cursor_y = 5;
  set_screen(get_colour(&text_info.background));
}

void video_putchar(char ch, int x, int y, colour_t colour) {
  for (uint8_t iy = 0; iy < 8; iy++) {
    for (uint8_t ix = 0; ix < 8; ix++) {
      if ((font[(uint8_t)ch][iy] >> ix) & 1) {
        uint64_t offset =
            ((iy + y) * fb_info->framebuffer_pitch) + ((ix + x) * 4);
        *(uint32_t *)((uint64_t)fb_info->framebuffer_addr + offset) =
            get_colour(&colour);
      }
    }
  }
}

void video_put(char c) {
  // TODO: Change to ANSI colour coding
  if (text_info.parsing_state == PARSING) {
    colour_t text_color = text_info.color_scheme[(int)c - 48];
    text_info.text = text_color;

    text_info.parsing_state = OFF;

    return;
  }

  switch (c) {
    case '\n':
      text_info.cursor_x = 5;
      text_info.cursor_y += 20;

      break;
    case '\0':
      text_info.cursor_x -= 1;
      video_putchar(' ', text_info.cursor_x, text_info.cursor_y,
                    text_info.text);
      text_info.cursor_x++;

      break;
    case '\033':
      text_info.parsing_state = PARSING;

      break;
    default:
      video_putchar(c, text_info.cursor_x, text_info.cursor_y, text_info.text);

      text_info.cursor_x += 8;
      if (text_info.cursor_x >= (size_t)fb_info->framebuffer_width - 5) {
        text_info.cursor_x = 5;
        text_info.cursor_y += 20;
      }

      break;
  }
}

void video_puts(char *string) {
  while (*string) {
    video_put(*string++);
  }

  text_info.text = rgb(255, 255, 255);
}

void video_printf(char *format, ...) {
  char buffer[1000] = {0};

  va_list args;
  va_start(args, format);

  vsprintf(buffer, format, args);

  va_end(args);

  video_puts(buffer);
}

void init_video(struct stivale2_struct *info) {
  struct stivale2_struct_tag_framebuffer *fb_hdr_tag;
  fb_hdr_tag = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

  if (fb_hdr_tag == NULL) {
    for (;;) {
      __asm__ volatile("hlt");
    }
  }

  fb_info = fb_hdr_tag;

  colour_t scheme[8] = {
      rgb(0, 0, 0),       /* Black */
      rgb(220, 50, 47),   /* Red */
      rgb(133, 153, 0),   /* Green */
      rgb(181, 137, 0),   /* Yellow */
      rgb(38, 139, 210),  /* Blue */
      rgb(108, 113, 196), /* Magenta */
      rgb(42, 161, 152),  /* Cyan */
      rgb(255, 255, 255)  /* White */
  };

  for (int i = 0; i < 8; i++) {
    text_info.color_scheme[i] = scheme[i];
  }

  text_info.background = rgb(0, 0, 0);
  text_info.text = rgb(255, 255, 255);
  text_info.cursor_x = 5;
  text_info.cursor_y = 5;
  text_info.parsing_state = OFF;

  set_screen(get_colour(&text_info.background));

  log(INFO, "Video initalized");
  log(INFO, "Resolution: %dx%d", fb_info->framebuffer_width,
      fb_info->framebuffer_height);
  log(INFO, "Pitch: %d", fb_info->framebuffer_pitch);
  log(INFO, "BPP: %d", fb_info->framebuffer_bpp);
}
