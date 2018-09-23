all:
	gcc -march=x86-64 -ffreestanding -Wall -m16 -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -Os -fno-common   -c -o main.o main.c
	as -march=i386 --32 -o boot.o boot.s
	ld -m elf_i386 -static -Tmain.ld -nostdlib --nmagic -o xana main.o boot.o # --print-map

start:
	qemu-system-x86_64 -enable-kvm -fda xana -serial stdio -monitor none -nographic
