CC=arm-linux-gnueabihf-gcc

EXCLUDED_PATHS = \
  -path "*/mnt/d/byteos/kernel/arch/x86_64/*" -o \
  -path "*/mnt/d/byteos/.vs/*" -o \
  -path "*/mnt/d/byteos/.vscode/*" -o \
  -path "*/.git/*"

BOOTARMFLAGS = -std=c17 -Wall -Wextra -Wpedantic -ffreestanding -nostartfiles -nostdlib -nodefaultlibs -mcpu=cortex-a15 -e EFI_MAIN
BOOTARMH=$(shell find /mnt/d/byteos -type d \( $(EXCLUDED_PATHS) \) -prune -o -type d -print | sed 's/^/-I /')
BOOTARMC=$(shell find /mnt/d/byteos -type f \( $(EXCLUDED_PATHS) \) -prune -o -name "*.c" -print)
BOOTARMS=$(shell find /mnt/d/byteos -type f \( $(EXCLUDED_PATHS) \) -prune -o -name "*.s" -print)

efimake:
	$(CC) $(BOOTARMFLAGS) $(BOOTARMH) $(BOOTARMC) $(BOOTARMS) -o /mnt/d/byteos/boot/BOOTARM.EFI

KERNELFLAGS= -ffreestanding -nostdlib -mcpu=cortex-a15 -e Kernel_Main
KERNELH=$(shell find /mnt/d/byteos/kernel -type d \( $(EXCLUDED_PATHS) \) -prune -o -type d -print | sed 's/^/-I /')
KERNELC=$(shell find /mnt/d/byteos/kernel -type f \( $(EXCLUDED_PATHS) \) -prune -o -name "*.c" -print)
KERNELS=$(shell find /mnt/d/byteos/kernel -type f \( $(EXCLUDED_PATHS) \) -prune -o -name "*.s" -print)

kernelmake:
	$(CC) $(KERNELFLAGS) $(KERNELH) $(KERNELC) $(KERNELS) -o /mnt/d/byteos/kernel/byteos-arm.bin

fullmake:
	$(CC) $(BOOTARMFLAGS) $(BOOTARMH) $(BOOTARMC) $(BOOTARMS) -o /mnt/d/byteos/boot/BOOTARM.EFI
	$(CC) $(KERNELFLAGS) $(KERNELS) $(KERNELH) $(KERNELC) -o /mnt/d/byteos/kernel/byteos-arm.bin
	/mnt/c/Windows/System32/WindowsPowerShell/v1.0/powershell.exe -ExecutionPolicy Bypass -file D:/byteos/run-arm.ps1