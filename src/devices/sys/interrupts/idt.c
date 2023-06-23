#include "idt.h"

static struct idt_descriptor idt[256];
static struct idt_pointer idtr = {.size = 256 * sizeof(struct idt_descriptor), .addr = (uint64_t)idt};

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
}

static struct idt_descriptor idt_make_entry(uint64_t offset) {
    return (struct idt_descriptor) {
        .selector = 0x08,
        .offset_lo = offset & 0xFFFF,
        .offset_mid = (offset >> 16) & 0xFFFF,
        .offset_hi = (offset >> 32) & 0xFFFFFFFF,
        .ist = 0,
        .zero = 0,
        .type_attr = 0x8e};
}

void isr_init() {
    pic_remap();
    /* Exception Handling */
    idt[0] = idt_make_entry((uint64_t)&exc_0);
    idt[1] = idt_make_entry((uint64_t)&exc_1);
    idt[2] = idt_make_entry((uint64_t)&exc_2);
    idt[3] = idt_make_entry((uint64_t)&exc_3);
    idt[4] = idt_make_entry((uint64_t)&exc_4);
    idt[5] = idt_make_entry((uint64_t)&exc_5);
    idt[6] = idt_make_entry((uint64_t)&exc_6);
    idt[7] = idt_make_entry((uint64_t)&exc_7);
    idt[8] = idt_make_entry((uint64_t)&exc_8);
    idt[10] = idt_make_entry((uint64_t)&exc_10);
    idt[11] = idt_make_entry((uint64_t)&exc_11);
    idt[12] = idt_make_entry((uint64_t)&exc_12);
    idt[13] = idt_make_entry((uint64_t)&exc_13);
    idt[14] = idt_make_entry((uint64_t)&exc_14);
    idt[15] = idt_make_entry((uint64_t)&exc_15);
    idt[16] = idt_make_entry((uint64_t)&exc_16);
    idt[17] = idt_make_entry((uint64_t)&exc_17);
    idt[18] = idt_make_entry((uint64_t)&exc_18);
    idt[19] = idt_make_entry((uint64_t)&exc_19);
    idt[20] = idt_make_entry((uint64_t)&exc_20);
    idt[30] = idt_make_entry((uint64_t)&exc_30);
    idt[33] = idt_make_entry((uint64_t)&keyboard_handler);

    int i, j;
    for (i = 0x23; i < 0x28; i++)
    {
        idt[i] = idt_make_entry((uint64_t)&isr_irq_master);
    }
    for (j = 0x28; j < 0x2F; j++)
    {
        idt[j] = idt_make_entry((uint64_t)&isr_irq_slave);
    }
}

void idt_load() {
    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr));
}

void init_idt() {
    isr_init();
    idt_load();
    __asm__ volatile("sti");
}