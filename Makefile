ASMFILES := $(shell find src/ lib/ -type f -name '*.asm')
CFILES := $(shell find src/ lib/ -type f -name '*.c')
OBJ := $(CFILES:.c=.o) $(ASMFILES:.asm=.o)
CC = clang-11
LD = ld
ASSEMBLER = nasm
KERNEL_HDD = build/disk.hdd
KERNEL_ELF = myos.elf

LDINTERNALFLAGS :=      \
	-T src/linker.ld    \
	-static             \
	-nostdlib           \
	-no-pie

CFLAGS :=                          \
	-Wall				           \
	-Wextra						   \
	-Werror                        \
	-pipe                          \
	-fno-pic                       \
	-mno-sse                       \
	-mno-sse2                      \
	-mno-mmx                       \
	-g                             \
	-mno-80387                     \
	-mno-red-zone                  \
	-mcmodel=kernel                \
	-ffreestanding                 \
	-fno-stack-protector           \
	-Isrc/                         \
	-Ilib/	

NASMFLAGS := -felf64

.PHONY: clean all run

all: $(KERNEL_HDD)

run: $(KERNEL_HDD)
	@echo [ QEMU ] Starting $(KERNEL_HDD)
	@qemu-system-x86_64 -vga std -drive file=$(KERNEL_HDD),format=raw -enable-kvm -serial stdio -rtc base=localtime -m 2G

%.o: %.c
	@echo [ CC ] $<
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	@echo [ NASM ] $<
	@$(ASSEMBLER) $(NASMFLAGS) $< -o $@

$(KERNEL_ELF): $(OBJ)
	@echo [ LD ] $<
	@$(LD) $(LDINTERNALFLAGS) $(OBJ) -o $@

limine/limine-install:
	@echo [ MAKE ] $<
	@-$(MAKE) -C limine/ limine-install

$(KERNEL_HDD): $(KERNEL_ELF) limine/limine-install
	@echo "[ MKDIR | RM ] build/ $(KERNEL_HDD)"
	@-mkdir build
	@rm -f $(KERNEL_HDD)

	@echo "[ DD | PARTED | ECHFS-UTILS ]" $(KERNEL_HDD)
	@dd if=/dev/zero bs=1M count=0 seek=64 of=$(KERNEL_HDD)
	@parted -s $(KERNEL_HDD) mklabel gpt
	@parted -s $(KERNEL_HDD) mkpart primary 2048s 100%
	@echfs-utils -g -p0 $(KERNEL_HDD) quick-format 512

	@echo [ ECHFS-UTILS ] $(KERNEL_ELF) limine.cfg
	@echfs-utils -g -p0 $(KERNEL_HDD) import $(KERNEL_ELF) $(KERNEL_ELF)
	@echfs-utils -g -p0 $(KERNEL_HDD) import limine.cfg limine.cfg

	@echo [ LIMINE ] $(KERNEL_HDD)
	@limine/limine-install $(KERNEL_HDD)

clean:
	@echo [ RM ] $(KERNEL_HDD) $(TARGET) $(OBJ)
	@rm -f $(KERNEL_HDD) $(TARGET) $(OBJ)
