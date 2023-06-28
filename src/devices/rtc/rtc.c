#include "rtc.h"

/* Check if RTC is updating */
static int is_updating(void) {
  outb(0x70, 0x0A);
  return inb(0x71) & 0x80;
}

static unsigned char read(int reg) {
  while (is_updating())
    ;

  outb(0x70, reg);
  return inb(0x71);
}

static unsigned char bcd_to_bin(unsigned char val) {
  return (val & 0xF) + (val >> 4) * 10;
}

datetime_t rtc_get_datetime(void) {
  datetime_t datetime;

  unsigned char second = read(0x00);
  unsigned char minute = read(0x02);
  unsigned char hour = read(0x04);
  unsigned char day = read(0x07);
  unsigned char month = read(0x08);
  unsigned char year = read(0x09);

  unsigned char regb = read(0x0b);

  if (~regb & 0x04) {
    second = bcd_to_bin(second);
    minute = bcd_to_bin(minute);
    hour = bcd_to_bin(hour);
    day = bcd_to_bin(day);
    month = bcd_to_bin(month);
    year = bcd_to_bin(year);
  }

  datetime.second = second;
  datetime.minute = minute;
  datetime.hour = hour;
  datetime.day = day;
  datetime.month = month;
  datetime.year = year;

  return datetime;
}

void init_rtc(void) {
  log(INFO, "RTC initalized");

  datetime_t current = rtc_get_datetime();
  log(INFO, "Date: %d/%d/%d", current.day, current.month, current.year);
  log(INFO, "Time: %d:%d:%d", current.hour, current.minute, current.second);
}
