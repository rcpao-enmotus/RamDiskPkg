/** @file
  TODO: Brief Description of UEFI Driver RamDisk
  
  TODO: Detailed Description of UEFI Driver RamDisk

  TODO: Copyright for UEFI Driver RamDisk
  
  TODO: License for UEFI Driver RamDisk

**/

#ifndef __EFI_RAM_DISK_H__
#define __EFI_RAM_DISK_H__

#include <Uefi.h>

//
// Libraries
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>

//
// UEFI Driver Model Protocols
//
#include <Protocol/DriverBinding.h>
#include <Protocol/DriverSupportedEfiVersion.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/ComponentName.h>

//
// Consumed Protocols
//
#include <Protocol/PciIo.h>

//
// Produced Protocols
//
#include <Protocol/BlockIo.h>
#include <Protocol/BlockIo2.h>

//
// Guids
//

//
// Driver Version
//
#define RAM_DISK_VERSION  0x00000000

//
// Protocol instances
//
extern EFI_DRIVER_BINDING_PROTOCOL  gRamDiskDriverBinding;
extern EFI_COMPONENT_NAME2_PROTOCOL  gRamDiskComponentName2;
extern EFI_COMPONENT_NAME_PROTOCOL  gRamDiskComponentName;

//
// Include files with function prototypes
//
#include "DriverBinding.h"
#include "ComponentName.h"
#include "BlockIo.h"

#endif
