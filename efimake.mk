CC=x86_64-w64-mingw32-gcc
CFLAGS=/mnt/d/byteos/boot/BootManager.c -std=c17 -Wall -Wextra -Wpedantic -mno-red-zone -ffreestanding -nostdlib -Wl,-subsystem,10 -e efi_main
INCLUDE=$(shell find /mnt/d/byteos -type d | sed 's/^/-I /')
SRC=$(shell find /mnt/d/byteos -type f -name "*.c")

efimake:
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o /mnt/d/byteos/boot/BOOTX64.EFI
