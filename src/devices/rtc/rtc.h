#ifndef RTC_H
#define RTC_H

#include <libasm/asm.h>
#include <liblog/logging.h>
#include <stdint.h>

typedef struct {
  uint32_t year;
  uint32_t month;
  uint32_t day;
  uint32_t hour;
  uint32_t minute;
  uint32_t second;
} datetime_t;

datetime_t rtc_get_datetime(void);
void init_rtc(void);

#endif
