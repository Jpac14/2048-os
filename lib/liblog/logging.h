#ifndef LOGGING_H
#define LOGGING_H

#include <devices/serial/serial.h>
#include <devices/video/video.h>
#include <stdarg.h>

enum Status { INFO, DEBUG, WARNING, ERROR, PANIC };

void log_serial(enum Status status, char *format, ...);
void log_video(enum Status status, char *format, ...);
void log(enum Status status, char *format, ...);

#endif
