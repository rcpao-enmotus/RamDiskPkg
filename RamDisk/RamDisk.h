/** @file
RamDisk -- UEFI RamDisk Driver
Copyright (C) 2016  Enmotus, Inc.

http://www.enmotus.com
65 Enterprise
Aliso Viejo, CA 92656
Phone: 949.330.6220
Info@enmotus.com
sales@enmotus.com
saleseurope@enmotus.com
Roger C. Pao <rcpao.enmotus+RamDiskEfi@gmail.com>


GNU General Public License, version 2

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; only version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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
//#include <Protocol/PciIo.h>

//
// Produced Protocols
//
#include <Protocol/BlockIo.h>
#include <Protocol/BlockIo2.h>

//
// Guids
//

//
// Custom Protocol GUID
//
#define EFI_RAM_DISK_DRIVER_PROTOCOL_GUID \
  {0xe8d377e1, 0xd042, 0x11e5, {0x85, 0x0b, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x08}}

//
// Driver Version
//
#define RAM_DISK_VERSION  0x10
  /* 
   * The version number of the EFI Driver that produced the
   * EFI_DRIVER_BINDING_PROTOCOL. This field is used by
   * the EFI boot service ConnectController() to determine
   * the order that driver’s Supported() service will be used
   * when a controller needs to be started. EFI Driver Binding
   * Protocol instances with higher Version values will be used
   * before ones with lower Version values. The Version values
   * of 0x0-0x0f and 0xfffffff0-0xffffffff are reserved
   * for platform/OEM specific drivers. The Version values of
   * 0x10-0xffffffef are reserved for IHV-developed drivers.
   */

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

#include "pr.h"


typedef struct _RAM_DISK_DEVICE_PATH {
  EFI_DEVICE_PATH Header;
  EFI_GUID Guid;
  UINT8 DiskId[8];
  EFI_DEVICE_PATH EndDevicePath;
} RAM_DISK_DEVICE_PATH;


#define RAM_DISK_SIGNATURE    SIGNATURE_32('R','a','m','D')
#define RAM_DISK_FROM_THIS(a) CR(a, RAM_DISK, BlockIoProtocol, RAM_DISK_SIGNATURE)
  /* RamDisk = RAM_DISK_FROM_THIS(This); finds the RamDisk structure given a RamDisk BlockIoProtocol interface */
typedef struct _RAM_DISK {
  UINTN Signature;
  EFI_BLOCK_IO_PROTOCOL BlockIoProtocol;
  EFI_BLOCK_IO_MEDIA BlockIoMedia;
  //unused: EFI_DEVICE_PATH *ParentDevicePath;
  EFI_DEVICE_PATH *DevicePath;
  unsigned char *RamBuffer; /* diskimage */
  unsigned int RamBufferSize; /* diskimage_len */
  EFI_LBA AddLbaOfs;
  BOOLEAN InstalledDevicePathProtocol;
  BOOLEAN InstalledBlockIoProtocol;
} RAM_DISK;


/* ---------------------------------------------------------------------------
Externals
--------------------------------------------------------------------------- */

#if (defined(__RAM_DISK_C))

#include "diskimage.h"
#define diskimage diskimage_img
#define diskimage_len diskimage_img_len


EFI_GUID gEfiRamDiskProtocolGuid = EFI_RAM_DISK_DRIVER_PROTOCOL_GUID;

static RAM_DISK_DEVICE_PATH RamDiskDevicePath = {
  {
    MESSAGING_DEVICE_PATH, /* Type */
    MSG_VENDOR_DP, /* SubType */
    {
      sizeof(RAM_DISK_DEVICE_PATH) - END_DEVICE_PATH_LENGTH, /* Length LSB */
      0, /* Length MSB */
    }
  },
  EFI_RAM_DISK_DRIVER_PROTOCOL_GUID,
  {0,0,0,0,0,0,0,0},	// DiskId assigned below
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {END_DEVICE_PATH_LENGTH, 0}
  }
};

RAM_DISK RamDisk;

#else /* #if (defined(__RAM_DISK_C)) */

extern unsigned char RamDisk_img[];
extern unsigned int RamDisk_img_len;
#define diskimage RamDisk_img
#define diskimage_len RamDisk_img_len

extern EFI_GUID gEfiRamDiskProtocolGuid;
extern RAM_DISK_DEVICE_PATH RamDiskDevicePath;
extern RAM_DISK RamDisk;

#endif /* #if (defined(__RAM_DISK_C)) */


#endif /* #ifndef __EFI_RAM_DISK_H__ */
