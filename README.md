# RamDiskPkg

https://github.com/tianocore/tianocore.github.io/wiki/UDK2014.IHV-Setup-Guide
https://github.com/tianocore-docs/Docs/raw/master/Driver_Developer/UDK2014.IHV.zip

UDK2014.IHV/Conf/target.txt
ACTIVE_PLATFORM       = RamDiskPkg/RamDiskPkg.dsc
TARGET_ARCH           = X64
TOOL_CHAIN_TAG        = VS2012x86

cd UDK2014.IHV
git clone git@github.com:rcpao-enmotus/RamDiskPkg.git

build
