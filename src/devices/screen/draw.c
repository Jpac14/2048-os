#include "draw.h"

extern struct psf fb_font;

void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
               colour_t colour) {
  int i, j;
  for (i = x; i < x + w; i++) {
    for (j = y; j < y + h; j++) {
      set_pixel(i, j, get_colour(&colour));
    }
  }
}

void font_putc(char c, uint16_t x, uint16_t y, uint8_t scale,
               colour_t foreground, colour_t background) {
  uint8_t *glyph = &fb_font.data[c * fb_font.glyph_size];

  static const uint8_t masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};

  uint16_t i, j;
  for (i = 0; i < fb_font.height; i++) {
    for (j = 0; j < fb_font.width; j++) {
      if (glyph[i] & masks[j]) {
        draw_rect(x + j * scale, y + i * scale, scale, scale, foreground);
      } else {
        draw_rect(x + j * scale, y + i * scale, scale, scale, background);
      }
    }
  }
}

void font_puts(char *string, uint16_t x, uint16_t y, uint8_t scale,
               colour_t foreground, colour_t background) {
  uint16_t cursor_x = x;
  while (*string) {
    font_putc(*string++, cursor_x, y, scale, foreground, background);
    cursor_x += fb_font.width * scale;
  }
}

void font_puts_center(char *string, uint16_t center_x, uint16_t center_y,
                      uint8_t scale, colour_t foreground, colour_t background) {
  unsigned int length = strlen(string);

  uint16_t x = center_x - ((length * fb_font.width * scale) / 2);
  uint16_t y = center_y - ((fb_font.height * scale) / 2);

  font_puts(string, x, y, scale, foreground, background);
}
