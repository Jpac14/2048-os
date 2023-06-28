#ifndef IDT_H
#define IDT_H

#include <devices/serial/serial.h>
#include <libasm/asm.h>
#include <liblog/logging.h>
#include <stdint.h>
#include <system/panic/panic.h>

#define GDT_OFFSET_KERNEL_CODE 0x08
#define IDT_MAX_DESCRIPTORS 256

extern void *isr_stub_table[];

struct idt_entry {
  uint16_t isr_low;    // The lower 16 bits of the ISR's address
  uint16_t kernel_cs;  // The GDT segment selector that the CPU will load into
                       // CS before calling the ISR
  uint8_t ist;  // The IST in the TSS that the CPU will load into RSP; set to
                // zero for now
  uint8_t attributes;  // Type and attributes; see the IDT page
  uint16_t
      isr_mid;  // The higher 16 bits of the lower 32 bits of the ISR's address
  uint32_t isr_high;  // The higher 32 bits of the ISR's address
  uint32_t reserved;  // Set to zero
} __attribute__((packed));

struct idt_pointer {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

void init_idt(void);

#endif
