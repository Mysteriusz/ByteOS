CC=clang
LD=lld

BOOTX64_EXCLUDED_PATHS = \
  -path "/mnt/d/byteos/kernel" -o \
  -path "/mnt/d/byteos/.vs" -o \
  -path "/mnt/d/byteos/.vscode" -o \
  -path "/mnt/d/byteos/.git"

BOOTX64FLAGS = -ffreestanding -MMD -mno-red-zone -std=c11 -target x86_64-unknown-windows -Wall -pedantic
BOOTX64FLAGS_LD = -flavor link -subsystem:efi_application -entry:EFI_MAIN
BOOTX64H=$(shell find /mnt/d/byteos/boot \( $(BOOTX64_EXCLUDED_PATHS) \) -prune -false -o -type d -print | sed 's/^/-I /')
BOOTX64C=$(shell find /mnt/d/byteos/boot \( $(BOOTX64_EXCLUDED_PATHS) \) -prune -false -o -name "*.c" -print)
BOOTX64S=$(shell find /mnt/d/byteos/boot \( $(BOOTX64_EXCLUDED_PATHS) \) -prune -false -o -name "*.s" -print)

%.o: %.c
	$(CC) $(BOOTX64FLAGS) $(BOOTX64H) $(BOOTX64S) -c $< -o $@

OBJS := $(BOOTX64C:.c=.o)

efimake: $(OBJS)
	$(LD) $(BOOTX64FLAGS_LD) $^ -out:/mnt/d/byteos/builds/bootx64.efi

run:
	/mnt/c/Windows/System32/WindowsPowerShell/v1.0/powershell.exe -ExecutionPolicy Bypass -file D:/byteos/builds/run-x86_64.ps1

clean:
	rm -f $(OBJS) $(BOOTX64C:.c=.d) /mnt/d/byteos/builds/bootx64.efi
	rm -f $(OBJS) /mnt/d/byteos/builds/bootx64.efi
	rm -f $(OBJS) $(KERNELC:.c=.d) /mnt/d/byteos/kernel/byteos-x86_64.bin
	rm -f $(OBJS) /mnt/d/byteos/kernel/byteos-x86_64.bin
