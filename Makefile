CFLAGS = -Wall -Wextra -march=x86-64 -ffreestanding -fno-pie\
		 -fno-stack-protector -fno-asynchronous-unwind-tables\
		 -Os -mno-sse -fno-common -Wimplicit-fallthrough=0
CPPFLAGS = -Iincludes
ASFLAGS = -march=i386 --32
LDFLAGS = -static -nostdlib --nmagic

VPATH = stage0 stage1 stage2 stage3 stage4

QEMU = qemu-system-x86_64
QEMUFLAGS = -fda $(TARGET) -enable-kvm

# OBJECT FILES
EXTRA_OBJECT ?= win.o
STAGE0_OBJS = boot.o init.o
STAGE1_OBJS = gdt.o
STAGE2_OBJS = idt.o isr.o idt_asm.o paging.o
STAGE3_OBJS = test.o printf.o
OBJS = $(STAGE0_OBJS) $(STAGE1_OBJS) $(STAGE2_OBJS) $(STAGE3_OBJS)
# !OBJECT FILES

STAGE0_ELF = stage0.elf
STAGE1_ELF = stage1.elf
STAGE2_ELF = stage2.elf
STAGE3_ELF = stage3.elf
STAGES_ELF = $(STAGE0_ELF) $(STAGE1_ELF) $(STAGE2_ELF) $(STAGE3_ELF)

TARGET = hello64

OBJCOPY = objcopy
OBJCOPYFLAGS = -O elf64-x86-64

# RULES
all: $(TARGET)

$(STAGE0_OBJS): CFLAGS += -m16
$(STAGE1_OBJS): CFLAGS += -m16
$(STAGE2_OBJS): CFLAGS += -m32
$(STAGE3_OBJS): CFLAGS += -m64

$(STAGE0_ELF): LDFLAGS += -Tstage0/stage0.ld -r -m elf_i386 -Map=$@.map
$(STAGE0_ELF): $(STAGE0_OBJS)
	$(LD) $(LDFLAGS) $^ -o $@
	$(OBJCOPY) $(OBJCOPYFLAGS) $@
$(STAGE1_ELF): LDFLAGS += -Tstage1/stage1.ld -r -m elf_i386 -Map=$@.map
$(STAGE1_ELF): $(STAGE1_OBJS)
	$(LD) $(LDFLAGS) $^ -o $@
	$(OBJCOPY) $(OBJCOPYFLAGS) $@
$(STAGE2_ELF): LDFLAGS += -Tstage2/stage2.ld -r -m elf_i386 -Map=$@.map
$(STAGE2_ELF): $(STAGE2_OBJS)
	$(LD) $(LDFLAGS) $^ -o $@
	$(OBJCOPY) $(OBJCOPYFLAGS) $@
$(STAGE3_ELF): LDFLAGS += -Tstage3/stage3.ld -r -Map=$@.map
$(STAGE3_ELF): $(STAGE3_OBJS) $(EXTRA_OBJECT)
	$(LD) $(LDFLAGS) $^ -o $@
	$(OBJCOPY) $(OBJCOPYFLAGS) $@

prepare: $(EXTRA_OBJECT)
	cp $^ stage3
step1: $(STAGE3_ELF) $(STAGE2_ELF) $(STAGE1_ELF)
step2: CFLAGS += -DSTAGE1_SIZE=$(shell du -b $(STAGE1_ELF) | cut -f1 | head -n1) \
	-DSTAGE2_SIZE=$(shell du -b $(STAGE2_ELF) | cut -f1 | head -n1) \
	-DSTAGE3_SIZE=$(shell du -b $(STAGE3_ELF) | cut -f1 | head -n1)
step2: $(STAGE0_ELF)

$(TARGET): prepare step1 step2
	$(LD) $(LDFLAGS) -Tmain.ld -Map=$(TARGET).map $(STAGES_ELF) -o $@

$(TARGET).elf: CFLAGS += -g
$(TARGET).elf: $(STAGES_ELF)
	$(LD) $(LDFLAGS) -Tmain.ld --oformat elf64-x86-64 $^ -o $@

qemu: $(TARGET)
	$(QEMU) $(QEMUFLAGS)

boot: QEMUFLAGS += -serial stdio -monitor none -nographic
boot: $(TARGET) qemu

# DEBUG
debug: QEMUFLAGS += -serial stdio
debug: $(TARGET) qemu

#gdb: QEMU = qemu-system-x86_64
gdb: QEMUFLAGS += -S -s -daemonize
gdb: $(TARGET).elf $(TARGET) qemu
	gdb -ex 'target remote localhost:1234'
# !DEBUG

clean:
	$(RM) $(OBJS) $(TARGET) $(TARGET).map $(TARGET).elf\
		$(STAGES_ELF) $(STAGE1_ELF).map $(STAGE2_ELF).map\
		$(STAGE3_ELF).map $(STAGE0_ELF).map

.PHONY: all qemu boot debug gdb clean step1 step2
