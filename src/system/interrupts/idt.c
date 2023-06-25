#include "idt.h"

__attribute__((aligned(0x10))) static struct idt_entry
    idt[256];  // Create an array of IDT entries; aligned for performance

static struct idt_pointer idtr;

void pic_remap(void) {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);

  outb(0x21, 0x20);
  outb(0xA1, 0x28);

  outb(0x21, 0x04);
  outb(0xA1, 0x02);

  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  outb(0x21, 0x0);
  outb(0xA1, 0x0);

  log(INFO, "PIC Remapped");
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
  struct idt_entry* descriptor = &idt[vector];

  descriptor->isr_low = (uint64_t)isr & 0xFFFF;
  descriptor->kernel_cs = GDT_OFFSET_KERNEL_CODE;
  descriptor->ist = 0;
  descriptor->attributes = flags;
  descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
  descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
  descriptor->reserved = 0;
}

void init_idt(void) {
  pic_remap();

  idtr.base = (uintptr_t)&idt[0];
  idtr.limit = (uint16_t)sizeof(struct idt_entry) * IDT_MAX_DESCRIPTORS - 1;

  for (uint8_t vector = 0; vector < 32; vector++) {
    idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
  }

  idt_set_descriptor(32, isr_stub_table[32], 0x8E);  // PIT handler
  idt_set_descriptor(33, isr_stub_table[33], 0x8E);  // Keyboard handler

  __asm__ volatile("lidt %0" : : "m"(idtr));  // load the new IDT
  __asm__ volatile("sti");                    // set the interrupt flag

  log(INFO, "Loaded IDT");
  log(INFO, "Enabled interrupts");
}
