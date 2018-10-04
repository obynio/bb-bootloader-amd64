CFLAGS = -Wall -Wextra -march=x86-64 -ffreestanding -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -Os -mno-sse -fno-common
CPPFLAGS = -Iincludes
ASFLAGS = -march=i386 --32
LDFLAGS = -m elf_i386 -static -nostdlib --nmagic -Map=$(TARGET).map

VPATH = stage0 stage1 stage2 stage3 stage4

QEMU = qemu-system-x86_64
QEMUFLAGS = -fda $(TARGET) -d cpu_reset

# OBJECT FILES
STAGE0_OBJS = boot.o init.o
STAGE1_OBJS = gdt.o
STAGE2_OBJS = idt.o isr.o idt_asm.o paging.o printf.o
OBJS = $(STAGE0_OBJS) $(STAGE1_OBJS) $(STAGE2_OBJS)
# !OBJECT FILES

TARGET = wispr


# RULES
all: $(TARGET)

$(STAGE0_OBJS): CFLAGS += -m16
$(STAGE1_OBJS): CFLAGS += -m16
$(STAGE2_OBJS): CFLAGS += -m32

$(TARGET): LDFLAGS += -Tmain.ld
$(TARGET): $(OBJS) main.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

$(TARGET).elf: CFLAGS += -g
$(TARGET).elf: LDFLAGS += -Tmain.ld --oformat elf32-i386
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

# !DEBUG

clean:
	$(RM) $(OBJS) $(TARGET) $(TARGET).map $(TARGET).elf

.PHONY: all qemu boot debug gdb clean
