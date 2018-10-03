CFLAGS = -Wall -Wextra -march=x86-64 -ffreestanding -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -Os -mno-sse -fno-common
CPPFLAGS = -Iincludes
ASFLAGS = -march=i386 --32
LDFLAGS = -m elf_i386 -static -nostdlib --nmagic -Map=$(TARGET).map

VPATH = stage0 stage1 stage2 stage3 stage4

QEMU = qemu-system-x86_64
QEMUFLAGS = -fda $(TARGET)

OBJS = boot.o init.o gdt.o idt.o isr.o idt_asm.o paging.o

TARGET = wispr

all: $(TARGET)

init.o: CFLAGS += -m16
gdt.o: CFLAGS += -m16
isr.o: CFLAGS += -m32
idt.o: CFLAGS += -m32
idt_asm.o: CFLAGS += -m32
paging.o: CFLAGS += -m32

$(TARGET): LDFLAGS += -Tmain.ld
$(TARGET): $(OBJS) main.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

$(TARGET).elf: LDFLAGS += -Tmain.ld --oformat elf64-x86-64
$(TARGET).elf: $(OBJS) main.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET).elf

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
# !DEBUG

clean:
	$(RM) $(OBJS) $(TARGET) $(TARGET).map $(TARGET).elf

.PHONY: debug
