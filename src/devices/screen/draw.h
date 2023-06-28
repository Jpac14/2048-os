#ifndef DRAW_H
#define DRAW_H

#include <devices/screen/psf.h>
#include <devices/screen/screen.h>
#include <liblog/logging.h>
#include <libstr/string.h>
#include <stdint.h>

void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, colour_t colour);
void font_putc(char c, uint16_t x, uint16_t y, uint8_t scale,
               colour_t foreground, colour_t background);
void font_puts(char *string, uint16_t x, uint16_t y, uint8_t scale,
               colour_t foreground, colour_t background);
void font_puts_center(char *string, uint16_t center_x, uint16_t center_y,
                      uint8_t scale, colour_t foreground, colour_t background);

#endif
