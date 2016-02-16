rem build-rom.bat

rem -l ClassCode 01h	        ;0F Base Class 01h=Mass storage controller
rem -f VendorId 01C44h		;PCI VendorId 1c44=Enmotus Inc
rem -i DeviceId 8000h		;PCI DeviceId 8000=8000 Storage IO Controller
EfiRom.exe -o RamDisk.rom -ec ..\Build\RamDiskPkg\DEBUG_VS2012x86\X64\RamDisk.efi -l 0x01 -f 0x1C44 -i 0x8000 -v
EfiRom.exe -d RamDisk.rom
