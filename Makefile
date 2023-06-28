ASMFILES := $(shell find src/ lib/ -type f -name '*.asm')
CFILES := $(shell find src/ lib/ -type f -name '*.c')
HEADERFILES := $(shell find src/ lib/ -type f -name '*.h')
OBJ := $(CFILES:.c=.o) $(ASMFILES:.asm=.o)
CC = clang-15
ASSEMBLER = nasm
FORMATTER = clang-format
LINTER = clang-tidy
KERNEL_HDD = build/2048.hdd
KERNEL_ELF = 2048.elf

LDINTERNALFLAGS :=    					 \
	-O2														 \
	-target x86_64-unknown-none 	 \
	-T src/linker.ld    					 \
	-static             					 \
	-nostdlib           					 \
	-fpie													 \
	-fno-pic

CFLAGS :=                        \
	-target x86_64-unknown-none		 \
	-Wall													 \
	-Wpedantic			           		 \
	-Wextra						   					 \
	-Werror                        \
	-nostdlib											 \
	-pipe                          \
	-fno-pic                       \
	-fno-strict-aliasing					 \
	-fno-omit-frame-pointer				 \
	-fno-stack-protector           \
	-mno-sse						 					 \
	-mno-sse2											 \
	-mno-mmx                       \
	-mno-80387                     \
	-mno-red-zone                  \
	-mcmodel=kernel                \
	-ffreestanding                 \
	-g                             \
	-O2														 \
	-Isrc/                         \
	-Ilib/	

NASMFLAGS := -felf64

.PHONY: clean format all run debug

all: $(KERNEL_HDD)

run: $(KERNEL_HDD)
	@echo [ QEMU ] Starting $(KERNEL_HDD)
	@qemu-system-x86_64 -vga std -drive file=$(KERNEL_HDD),format=raw -enable-kvm -serial stdio -rtc base=localtime -m 256

debug: $(KERNEL_HDD)
	@echo [ QEMU ] Debugging $(KERNEL_HDD)
	@qemu-system-x86_64 -vga std -drive file=$(KERNEL_HDD),format=raw -d int -M smm=off -no-reboot -no-shutdown -serial stdio -rtc base=localtime -m 256

%.o: %.c
	@echo [ CC ] $<
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	@echo [ NASM ] $<
	@$(ASSEMBLER) $(NASMFLAGS) $< -o $@

$(KERNEL_ELF): $(OBJ)
	@echo [ LD ] $<
	@$(CC) $(LDINTERNALFLAGS) $(OBJ) -o $@

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
	@echo [ RM ] $(KERNEL_HDD) $(KERNEL_ELF) $(TARGET) $(OBJ)
	@rm -f $(KERNEL_HDD) $(KERNEL_ELF) $(TARGET) $(OBJ)

format: 
	@echo [ FORMAT ] $(CFILES) $(HEADERFILES)
	@$(FORMATTER) -i $(CFILES) $(HEADERFILES)

lint:
	@echo [ LINT ] $(CFILES) $(HEADERFILES)
	@$(LINTER) -checks='*' --warnings-as-errors='*' $(CFILES) $(HEADERFILES)