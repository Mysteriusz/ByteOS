$VHDX = "D:\byteos\boot\disk.vhdx"
$IMG = "D:\byteos\boot\disk.img"
$BIOS = "D:\byteos\boot\bios\bios-arm.bin"

$EFIFILESRC = "D:\byteos\boot\BOOTARM.EFI"
$EFIFILEDEST = "EFI\BOOT\BOOTARM.EFI"

$KERNELFILESRC = "D:\byteos\kernel\byteos-arm.bin"
$KERNELFILEDEST = "KERNEL\byteos-arm.bin"

$Mount = Mount-VHD -Path $VHDX -Passthru
$DriveLetter = ($Mount | Get-Disk | Get-Partition | Get-Volume).DriveLetter + ":"
Start-Sleep -Seconds 2

Copy-Item -Path "$EFIFILESRC" -Destination "$DriveLetter\$EFIFILEDEST" -Force
Copy-Item -Path "$KERNELFILESRC" -Destination "$DriveLetter\$KERNELFILEDEST" -Force

Write-Host "File copied successfully to $DriveLetter"
Dismount-VHD -Path $VHDX
Write-Host "VHDX unmounted!"

qemu-img convert -f vhdx -O raw "$VHDX" "$IMG"
qemu-system-arm -drive format=raw,unit=0,file="$IMG" -bios "$BIOS" -m 2G -name BYTEOS -machine virt -cpu cortex-a15 -net none 