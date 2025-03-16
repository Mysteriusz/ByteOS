$VHDX = "D:\byteos\boot\disk.vhdx"
$IMG = "D:\byteos\boot\disk.img"
$BIOS = "D:\byteos\boot\Bios64.bin"

$EFIFILESRC = "D:\byteos\boot\BOOTX64.EFI"
$EFIFILEDEST = "EFI\BOOT\BOOTX64.EFI"

$KERNELFILESRC = "D:\byteos\kernel\byteos.bin"
$KERNELFILEDEST = "KERNEL\byteos.bin"

$Mount = Mount-VHD -Path $VHDX -Passthru
$DriveLetter = ($Mount | Get-Disk | Get-Partition | Get-Volume).DriveLetter + ":"
Start-Sleep -Seconds 2

Copy-Item -Path "$EFIFILESRC" -Destination "$DriveLetter\$EFIFILEDEST" -Force
Copy-Item -Path "$KERNELFILESRC" -Destination "$DriveLetter\$KERNELFILEDEST" -Force

Write-Host "File copied successfully to $DriveLetter"
Dismount-VHD -Path $VHDX
Write-Host "VHDX unmounted!"

qemu-img convert -f vhdx -O raw "$VHDX" "$IMG"
qemu-system-x86_64 -drive format=raw,unit=0,file="$IMG" -bios "$BIOS" -m 256M -vga std -name BYTEOS -machine q35 -net none 