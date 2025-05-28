$VHDX = "D:\byteos\boot\disk.vhdx"
$IMG = "D:\byteos\boot\disk.img"
$BIOS = "D:\byteos\boot\bios\bios-x86_64.bin"

$EFIFILESRC = "D:\byteos\builds\bootx64.efi"
$EFIFILEDEST = "efi\boot\bootx64.efi"

$KERNELFILESRC = "D:\byteos\kernel\byteos-x86_64.bin"
$KERNELFILEDEST = "kernel\byteos-x86_64.bin"

$Temp = [System.IO.Path]::GetTempFileName()

#	Mount the disk

$Mount = @"
	select vdisk file="$VHDX"
	attach vdisk
"@

Set-Content -Path $Temp -Value $Mount

diskpart /s $Temp

#	Copy files to disk

Start-Sleep -Seconds 2

$DriveLetter = (Get-Volume | Where-Object {$_.DriveType -eq 'Fixed'} | Sort-Object DriveLetter -Descending | Select-Object -First 1).DriveLetter + ":"	

echo "$DriveLetter\$EFIFILEDEST"
echo "$VHDX"

Copy-Item -Path "$EFIFILESRC" -Destination "$DriveLetter\$EFIFILEDEST" -Force

Start-Sleep -Seconds 2

#	UnMount the disk

$UnMount = @"
	select vdisk file="$VHDX"
	detach vdisk
"@

Set-Content -Path $Temp -Value $UnMount
diskpart /s $Temp

Start-Sleep -Seconds 2

Remove-Item $Temp

qemu-img convert -f vhdx -O raw "$VHDX" "$IMG"
qemu-system-x86_64 -drive format=raw,unit=0,file="$IMG" -bios "$BIOS" -m 256M -vga std -name BYTEOS-X86_64 -machine q35 -net none 
