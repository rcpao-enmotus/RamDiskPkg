rem build-rom.bat

set IFNAME=..\Build\RamDiskPkg\DEBUG_VS2012x86\X64\RamDisk.efi
dir %IFNAME%


rem Enmotus MicroTiering SAS Adapter (Bobcat)
rem -l ClassCode 01h	        ;0F Base Class 01h=Mass storage controller
rem -f VendorId 01C44h		;PCI VendorId 1c44=Enmotus Inc
rem -i DeviceId 8000h		;PCI DeviceId 8000=8000 Storage IO Controller
rem EfiRom.exe -o RamDisk.rom -ec %IFNAME% -l 0x01 -f 0x1C44 -i 0x8000 -v


rem The Option ROM's ClassCode, VendorId, & DeviceId does not need to match the 
rem PCIe adapter's Configuration Registers.
rem RamDisk.rom Option ROM will still be loaded (while Secure Boot is disabled).
rem -l ClassCode 00h	        ;0F Base Class 00h=Unclassified device
rem -f VendorId 01C44h		;PCI VendorId FFFF=Illegal Vendor ID
rem -i DeviceId 8000h		;PCI DeviceId FFFF=
EfiRom.exe -o RamDisk.rom -ec %IFNAME% -l 0x00 -f 0xFFFF -i 0xFFFF -v
rem EfiRom tool start.
rem Processing EFI file    ..\Build\RamDiskPkg\DEBUG_VS2012x86\X64\RamDisk.efi
rem   Got subsystem = 0xB from image
rem   File size   = 0x2D2220
rem   Comp size   = 0x78E4D
rem   Output size = 0x79000
rem EfiRom tool done with return code is 0x0.


EfiRom.exe -d RamDisk.rom
rem Image 1 -- Offset 0x0
rem   ROM header contents
rem     Signature              0xAA55
rem     PCIR offset            0x001C
rem     Signature               PCIR
rem     Vendor ID               0xFFFF
rem     Device ID               0xFFFF
rem     Length                  0x001C
rem     Revision                0x0003
rem     DeviceListOffset        0x00
rem     Class Code              0x000000
rem     Image size              0x79000
rem     Code revision:          0x0000
rem     MaxRuntimeImageLength   0x00
rem     ConfigUtilityCodeHeaderOffset 0x00
rem     DMTFCLPEntryPointOffset 0x00
rem     Indicator               0x80   (last image)
rem     Code type               0x03   (EFI image)
rem   EFI ROM header contents
rem     EFI Signature          0x0EF1
rem     Compression Type       0x0001 (compressed)
rem     Machine type           0x8664 (unknown)
rem     Subsystem              0x000B (EFI boot service driver)
rem     EFI image offset       0x0038 (@0x38)
