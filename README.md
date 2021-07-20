# bb-bootloader-amd64

> A x86_64 Hello World, from bootloader to printf

The main objective is to write a little bootloader abd kernel code that will write on the serial line from x86_64 code.
* `stage0` and `stage1` are for real mode
* `stage2` is for protected mode
* `stage3` is for long mode

## Usage

A simple `make` and `make boot` should do the trick. Make sure to have all the dependencies installed.
The printf call is made in a separate `win.c` file linked with the bootloader.

## Paging

This bootloader is using 2-Mbyte page translation for long mode. Please refer to the AMD programmer's manual as it provides a simpler paging documentation than the Intel manual. The virtual address fields are described as follows.
* Bits 63:48 are a sign extension of bit 47 as required for canonical address forms.
* Bits 47:39 index into the 512-entry page-map level-4 table.
* Bits 38:30 index into the 512-entry page-directory-pointer table.
* Bits 29:21 index into the 512-entry page-directory table.
* Bits 20:0 provide the byte offset into the physical page.


![2mb-page-translation](https://user-images.githubusercontent.com/2095991/48156759-5448d600-e2ce-11e8-96bc-8c02d70ec040.png)

The following diagram shows the translation table format used in this bootloader.

![structures](https://user-images.githubusercontent.com/2095991/48157101-0b455180-e2cf-11e8-95d5-782d4b571430.png)


## This is a must-read

* Intel Architecture Software Developer's Manual Volume 3, section 4.1.2
* Intel Architecture Software Developer's Manual Volume 3, section 4.5.4
* AMD64 Architecture Programmer’s Manual Volume 3, section 5.3

