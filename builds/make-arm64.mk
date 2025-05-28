CC=clang
LD=lld

BOOTAA64_EXCLUDED_PATHS = \
  -path "~/byteos/kernel" -o \
  -path "~/byteos/.vs" -o \
  -path "~/byteos/.vscode" -o \
  -path "~/byteos/.git"

BOOTAA64FLAGS = -ffreestanding -MMD -mno-red-zone -std=c11 -target aarch64-unknown-windows -Wall -pedantic
BOOTAA64FLAGS_LD = -flavor link -subsystem:efi_application -entry:EFI_MAIN
BOOTAA64H=$(shell find ~/byteos/boot \( $(BOOTAA64_EXCLUDED_PATHS) \) -prune -false -o -type d -print | sed 's/^/-I /')
BOOTAA64C=$(shell find ~/byteos/boot \( $(BOOTAA64_EXCLUDED_PATHS) \) -prune -false -o -name "*.c" -print)
BOOTAA64S=$(shell find ~/byteos/boot \( $(BOOTAA64_EXCLUDED_PATHS) \) -prune -false -o -name "*.s" -print)

%.o: %.c
	$(CC) $(BOOTAA64FLAGS) $(BOOTAA64H) $(BOOTAA64S) -c $< -o $@

OBJS := $(BOOTAA64C:.c=.o)

efimake: $(OBJS)
	$(LD) $(BOOTAA64FLAGS_LD) $^ -out:/home/main/byteos/builds/bootaa64.efi

clean:
	rm -f $(OBJS) $(BOOTAA64C:.c=.d) ~/byteos/builds/bootaa64.efi
	rm -f $(OBJS) ~/byteos/builds/bootaa64.efi
	rm -f $(OBJS) $(KERNELC:.c=.d) ~/byteos/kernel/byteos-arm64.bin
	rm -f $(OBJS) ~/byteos/kernel/byteos-arm64.bin
