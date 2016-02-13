RamDisk -- RamDisk UEFI Driver

https://github.com/rcpao-enmotus/RamDiskPkg
Roger C. Pao <rcpao.enmotus+RamDiskEfi@gmail.com>


This version compiles diskimage.h directly into RamDisk.efi.

RamDisk.img can be mounted by ImDisk Virtual Disk Driver available from 
http://www.ltr-data.se/opencode.html/
Use xxd-i.sh to generate diskimage.h from RamDisk.img.


Build Instructions

https://github.com/tianocore/tianocore.github.io/wiki/UDK2014.IHV-Setup-Guide
https://github.com/tianocore-docs/Docs/raw/master/Driver_Developer/UDK2014.IHV.zip

UDK2014.IHV/Conf/target.txt
ACTIVE_PLATFORM       = RamDiskPkg/RamDiskPkg.dsc
TARGET_ARCH           = X64
TOOL_CHAIN_TAG        = VS2012x86 # or GCC##

cd UDK2014.IHV
git clone git@github.com:rcpao-enmotus/RamDiskPkg.git

build

RamDisk.efi locations:
UDK2014.IHV\Build\RamDiskPkg\DEBUG_VS2012x86\X64\RamDisk.efi
UDK2014.IHV\Build\RamDiskPkg\DEBUG_VS2012x86\X64\RamDiskPkg\RamDisk\RamDisk\DEBUG\RamDisk.efi
UDK2014.IHV\Build\RamDiskPkg\DEBUG_VS2012x86\X64\RamDiskPkg\RamDisk\RamDisk\OUTPUT\RamDisk.efi
