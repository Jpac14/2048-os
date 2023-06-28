global fb_font

section .data
align 8
fb_font:
	incbin "src/devices/screen/fb_font.psf"