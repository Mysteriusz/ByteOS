CC=x86_64-w64-mingw32-gcc

BOOTX64FLAGS= -std=c17 -Wall -Wextra -Wpedantic -mno-red-zone -ffreestanding -nostdlib -Wl,-subsystem,10 -e EFI_MAIN
BOOTX64H=$(shell find /mnt/d/byteos -type d -not -path '*/.git/*' | sed 's/^/-I /')
BOOTX64C=$(shell find /mnt/d/byteos -type f -not -path '*/.git/*' -name "*.c")

efimake:
	$(CC) $(BOOTX64FLAGS) $(BOOTX64H) $(BOOTX64C) -o /mnt/d/byteos/boot/BOOTX64.EFI

KERNELFLAGS= -mno-red-zone -ffreestanding -nostdlib -e Kernel_Main
KERNELH=$(shell find /mnt/d/byteos/kernel -type d | sed 's/^/-I /')
KERNELC=$(shell find /mnt/d/byteos/kernel -type f -name "*.c")

kernelmake:
	$(CC) $(KERNELFLAGS) $(KERNELH) $(KERNELC) -o /mnt/d/byteos/kernel/byteos.bin

fullmake:
	$(CC) $(BOOTX64FLAGS) $(BOOTX64H) $(BOOTX64C) -o /mnt/d/byteos/boot/BOOTX64.EFI
	$(CC) $(KERNELFLAGS) $(KERNELH) $(KERNELC) -o /mnt/d/byteos/kernel/byteos.bin