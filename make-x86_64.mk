CC=x86_64-w64-mingw32-gcc

EXCLUDED_PATHS=\
  -path "/mnt/d/byteos/kernel/arch/aarch32" -o \
  -path "/mnt/d/byteos/.vs" -o \
  -path "/mnt/d/byteos/.vscode" -o \
  -path "/mnt/d/byteos/.git"

BOOTX64FLAGS= -std=c17 -Wall -Wextra -Wpedantic -mno-red-zone -ffreestanding -nostdlib -Wl,-subsystem,10 -e EFI_MAIN
BOOTX64H=$(shell find /mnt/d/byteos \( $(EXCLUDED_PATHS) \) -prune -false -o -type d -print | sed 's/^/-I /')
BOOTX64C=$(shell find /mnt/d/byteos \( $(EXCLUDED_PATHS) \) -prune -false -o -name "*.c" -print)
BOOTX64S=$(shell find /mnt/d/byteos \( $(EXCLUDED_PATHS) \) -prune -false -o -name "*.s" -print)

efimake:
	$(CC) $(BOOTX64FLAGS) $(BOOTX64H) $(BOOTX64C) $(BOOTX64S) -o /mnt/d/byteos/boot/BOOTX64.EFI

KERNELFLAGS= -mno-red-zone -ffreestanding -nostdlib -e Kernel_Main
KERNELH=$(shell find /mnt/d/byteos/kernel \( $(EXCLUDED_PATHS) \) -prune -false -o -type d -print | sed 's/^/-I /')
KERNELC=$(shell find /mnt/d/byteos/kernel \( $(EXCLUDED_PATHS) \) -prune -false -o -name "*.c" -print)
KERNELS=$(shell find /mnt/d/byteos/kernel \( $(EXCLUDED_PATHS) \) -prune -false -o -name "*.s" -print)

kernelmake:
	$(CC) $(KERNELFLAGS) $(KERNELH) $(KERNELC) $(KERNELS) -o /mnt/d/byteos/kernel/byteos-x86_64.bin

fullmake:
	$(CC) $(BOOTX64FLAGS) $(BOOTX64H) $(BOOTX64C) $(BOOTX64S) -o /mnt/d/byteos/boot/BOOTX64.EFI
	$(CC) $(KERNELFLAGS) $(KERNELH) $(KERNELC) $(KERNELS) -o /mnt/d/byteos/kernel/byteos-x86_64.bin
	/mnt/c/Windows/System32/WindowsPowerShell/v1.0/powershell.exe -ExecutionPolicy Bypass -file D:/byteos/run-x86_64.ps1