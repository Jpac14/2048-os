KERNEL_HDD = build/disk.hdd
KERNEL_ELF = myos.elf

CC = gcc
LD = ld

CFLAGS = -Wall -Wextra -O2 -pipe

LDINTERNALFLAGS := \
	-T src/linker.ld    \
	-static        \
	-nostdlib      \
	-no-pie

INTERNALCFLAGS  :=       \
	-I.                  \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic             \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mcmodel=kernel      \
	-mno-red-zone        \
	-Isrc/               \
	-Ilib/ 	

CFILES := $(shell find src/ lib/ -type f -name '*.c')
OBJ := $(CFILES:.c=.o)

.PHONY: clean all run

all: $(KERNEL_HDD)

run: $(KERNEL_HDD)
	qemu-system-x86_64 -vga std -drive file=$(KERNEL_HDD),format=raw -enable-kvm -serial stdio -rtc base=localtime -m 2G

$(KERNEL_ELF): $(OBJ)
	$(LD) $(LDINTERNALFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

$(KERNEL_HDD): $(KERNEL_ELF)
	-mkdir build
	rm -f $(KERNEL_HDD)
	dd if=/dev/zero bs=1M count=0 seek=64 of=$(KERNEL_HDD)
	parted -s $(KERNEL_HDD) mklabel gpt
	parted -s $(KERNEL_HDD) mkpart primary 2048s 100%
	echfs-utils -g -p0 $(KERNEL_HDD) quick-format 512
	echfs-utils -g -p0 $(KERNEL_HDD) import $(KERNEL_ELF) $(KERNEL_ELF)
	echfs-utils -g -p0 $(KERNEL_HDD) import limine.cfg limine.cfg
	../emerald/limine/limine-install $(KERNEL_HDD)

clean:
	rm -f $(KERNEL_HDD)
	rm -rf $(TARGET) $(OBJ)
