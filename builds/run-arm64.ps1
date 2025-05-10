$VHDX = "D:\byteos\boot\disk.vhdx"
$IMG = "D:\byteos\boot\disk.img"
$BIOS = "D:\byteos\boot\bios\bios-arm64.bin"

$EFIFILESRC = "D:\byteos\builds\bootaa64.efi"
$EFIFILEDEST = "efi\boot\bootaa64.efi"

$KERNELFILESRC = "D:\byteos\kernel\byteos-arm64.bin"
$KERNELFILEDEST = "kernel\byteos-arm64.bin"

$Mount = Mount-VHD -Path $VHDX -Passthru
$DriveLetter = ($Mount | Get-Disk | Get-Partition | Get-Volume).DriveLetter + ":"
Start-Sleep -Seconds 2

Copy-Item -Path "$EFIFILESRC" -Destination "$DriveLetter\$EFIFILEDEST" -Force
Copy-Item -Path "$KERNELFILESRC" -Destination "$DriveLetter\$KERNELFILEDEST" -Force

Write-Host "File copied successfully to $DriveLetter"
Dismount-VHD -Path $VHDX
Write-Host "VHDX unmounted!"

qemu-img convert -f vhdx -O raw "$VHDX" "$IMG"
qemu-system-aarch64 -drive format=raw,unit=0,file="$IMG" -bios "$BIOS" -m 1G -device ramfb -machine virt -cpu cortex-a76 -name BYTEOS-ARM64 -net none -device qemu-xhci -device usb-kbd 