CFLAGS = -march=x86-64 -ffreestanding -Wall -m16 -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -Os -fno-common

ASFLAGS = -march=i386 --32

LDFLAGS = -m elf_i386 -static -nostdlib --nmagic

QEMU = qemu-system-x86_64
QEMUFLAGS = -enable-kvm -fda $(TARGET)

OBJS = stage0.o stage1.o idt.o boot.o
TARGET = wispr

all: $(TARGET)

$(TARGET): LDFLAGS += -Tmain.ld
$(TARGET): $(OBJS) main.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

qemu: $(TARGET)
	$(QEMU) $(QEMUFLAGS)

boot: QEMUFLAGS += -serial stdio -monitor none -nographic
boot: $(TARGET) qemu

debug: QEMUFLAGS += -serial stdio
debug: $(TARGET) qemu

gdb: QEMUFLAGS += -S -s -daemonize
gdb: $(TARGET) qemu
	gdb -ex 'target remote localhost:1234'

clean:
	$(RM) $(OBJS) $(TARGET)
