CFLAGS = -Wall -Wextra -march=x86-64 -ffreestanding -Wall -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -Os -fno-common
ASFLAGS = -march=i386 --32
LDFLAGS = -m elf_i386 -static -nostdlib --nmagic -Map=$(TARGET).map

VPATH = src

QEMU = qemu-system-x86_64
QEMUFLAGS = -fda $(TARGET)

OBJS = stage0.o gdt.o idt.o boot.o 32bit.o
TARGET = wispr

all: $(TARGET)

stage0.o: CFLAGS += -m16
gdt.o: CFLAGS += -m16
idt.o: CFLAGS += -m32

$(TARGET): LDFLAGS += -Tmain.ld
$(TARGET): $(OBJS) main.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

elftest: LDFLAGS += -Tmain.ld
elftest: $(OBJS) main.ld
	$(LD) $(LDFLAGS) --oformat elf64-x86-64 $(OBJS) -o elftest

qemu: $(TARGET)
	$(QEMU) $(QEMUFLAGS)

boot: QEMUFLAGS += -serial stdio -monitor none -nographic -enable-kvm
boot: $(TARGET) qemu

# DEBUG
debug: QEMUFLAGS += -serial stdio -enable-kvm
debug: $(TARGET) qemu

gdb: QEMU = qemu-system-i386
gdb: QEMUFLAGS += -S -s -daemonize
gdb: $(TARGET) qemu
	gdb -ex 'set architecture i8086'\
		-ex 'target remote localhost:1234'\
		-ex 'b *0x7c00'\
		-ex 'continue'
# /DEBUG

clean:
	$(RM) $(OBJS) $(TARGET) $(TARGET).map elftest

.PHONY: debug
