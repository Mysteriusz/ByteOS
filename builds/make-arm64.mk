CC=clang
LD=lld

BOOTAA64_EXCLUDED_PATHS = \
  -path "/mnt/d/byteos/kernel" -o \
  -path "/mnt/d/byteos/.vs" -o \
  -path "/mnt/d/byteos/.vscode" -o \
  -path "/mnt/d/byteos/.git"

BOOTAA64FLAGS = -ffreestanding -MMD -mno-red-zone -std=c11 -target aarch64-unknown-windows -Wall -pedantic
BOOTAA64FLAGS_LD = -flavor link -subsystem:efi_application -entry:EFI_MAIN
BOOTAA64H=$(shell find /mnt/d/byteos/boot \( $(BOOTAA64_EXCLUDED_PATHS) \) -prune -false -o -type d -print | sed 's/^/-I /')
BOOTAA64C=$(shell find /mnt/d/byteos/boot \( $(BOOTAA64_EXCLUDED_PATHS) \) -prune -false -o -name "*.c" -print)
BOOTAA64S=$(shell find /mnt/d/byteos/boot \( $(BOOTAA64_EXCLUDED_PATHS) \) -prune -false -o -name "*.s" -print)

%.o: %.c
	$(CC) $(BOOTAA64FLAGS) $(BOOTAA64H) $(BOOTAA64S) -c $< -o $@

OBJS := $(BOOTAA64C:.c=.o)

efimake: $(OBJS)
	$(LD) $(BOOTAA64FLAGS_LD) $^ -out:/mnt/d/byteos/builds/bootaa64.efi

run:
	/mnt/c/Windows/System32/WindowsPowerShell/v1.0/powershell.exe -ExecutionPolicy Bypass -file D:/byteos/builds/run-arm64.ps1

clean:
	rm -f $(OBJS) $(BOOTAA64C:.c=.d) /mnt/d/byteos/builds/bootaa64.efi
	rm -f $(OBJS) /mnt/d/byteos/builds/bootaa64.efi
	rm -f $(OBJS) $(KERNELC:.c=.d) /mnt/d/byteos/kernel/byteos-arm64.bin
	rm -f $(OBJS) /mnt/d/byteos/kernel/byteos-arm64.bin
