#ifndef PSF_H
#define PSF_H

#include <stdint.h>

#define FONT_WIDTH 6
#define FONT_HEIGHT 12

struct psf {
  uint32_t magic;
  uint32_t version;
  uint32_t headersize;
  uint32_t flags;

  uint32_t numglyph;
  uint32_t glyph_size;
  uint32_t height;
  uint32_t width;

  uint8_t data[];
};

#endif
