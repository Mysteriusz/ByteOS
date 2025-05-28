CC=clang
LD=lld

BOOTX64_EXCLUDED_PATHS = \
  -path "~/byteos/kernel" -o \
  -path "~/byteos/.vs" -o \
  -path "~/byteos/.vscode" -o \
  -path "~/byteos/.git"

BOOTX64FLAGS = -ffreestanding -MMD -mno-red-zone -std=c11 -target x86_64-unknown-windows -Wall -pedantic
BOOTX64FLAGS_LD = -flavor link -subsystem:efi_application -entry:EFI_MAIN
BOOTX64H=$(shell find ~/byteos/boot \( $(BOOTX64_EXCLUDED_PATHS) \) -prune -false -o -type d -print | sed 's/^/-I /')
BOOTX64C=$(shell find ~/byteos/boot \( $(BOOTX64_EXCLUDED_PATHS) \) -prune -false -o -name "*.c" -print)
BOOTX64S=$(shell find ~/byteos/boot \( $(BOOTX64_EXCLUDED_PATHS) \) -prune -false -o -name "*.s" -print)

%.o: %.c
	$(CC) $(BOOTX64FLAGS) $(BOOTX64H) $(BOOTX64S) -c $< -o $@

OBJS := $(BOOTX64C:.c=.o)

efimake: $(OBJS)
	$(LD) $(BOOTX64FLAGS_LD) $^ -out:/home/main/byteos/builds/bootx64.efi

clean:
	rm -f $(OBJS) $(BOOTX64C:.c=.d) ~/byteos/builds/bootx64.efi
	rm -f $(OBJS) ~/byteos/builds/bootx64.efi
	rm -f $(OBJS) $(KERNELC:.c=.d) ~/byteos/kernel/byteos-x86_64.bin
	rm -f $(OBJS) ~/byteos/kernel/byteos-x86_64.bin
