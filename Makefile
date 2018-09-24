CFLAGS = -march=x86-64 -ffreestanding -Wall -m16 -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -Os -fno-common

ASFLAGS = -march=i386 --32

LDFLAGS = -m elf_i386 -static -nostdlib --nmagic

QEMU = qemu-system-x86_64
QEMUFLAGS = -enable-kvm -fda $(TARGET) -serial stdio -monitor none -nographic

OBJS = main.o boot.o
TARGET = xana

all: $(TARGET)

$(TARGET): LDFLAGS += -Tmain.ld
$(TARGET): $(OBJS) main.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

start: $(TARGET)
	$(QEMU) $(QEMUFLAGS)

clean:
	$(RM) $(OBJS) $(TARGET)
