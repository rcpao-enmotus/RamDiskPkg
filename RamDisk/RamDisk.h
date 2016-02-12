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

#include "pr.h"


typedef struct _RAM_DISK_DEVICE_PATH {
  EFI_DEVICE_PATH Header;
  EFI_GUID Guid;
  UINT8 DiskId[8];
  EFI_DEVICE_PATH EndDevicePath;
} RAM_DISK_DEVICE_PATH;

typedef struct _RAM_DISK {
  EFI_BLOCK_IO_PROTOCOL BlockIoProtocol;
  EFI_BLOCK_IO_MEDIA BlockIoMedia;
  EFI_DEVICE_PATH *ParentDevicePath;
  EFI_DEVICE_PATH *DevicePath;
  unsigned char *RamBuffer; /* diskimage */
  unsigned int RamBufferSize; /* diskimage_len */
  EFI_LBA AddLbaOfs;
  BOOLEAN Installed;
} RAM_DISK;


/* ---------------------------------------------------------------------------
Externals
--------------------------------------------------------------------------- */

#if (defined(__RAM_DISK_C))

#include "diskimage.h"
#define diskimage RamDisk_img
#define diskimage_len RamDisk_img_len


static RAM_DISK_DEVICE_PATH RamDiskDevicePath = {
  {
    MESSAGING_DEVICE_PATH, /* Type */
    MSG_VENDOR_DP, /* SubType */
    {
      sizeof(RAM_DISK_DEVICE_PATH) - END_DEVICE_PATH_LENGTH, /* Length LSB */
      0, /* Length MSB */
    }
  },
  /* GUID {e8d377e1-d042-11e5-850b-005056c00008} */
  {0xe8d377e1, 0xd042, 0x11e5, {0x85, 0x0b, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x08}},
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

extern RAM_DISK_DEVICE_PATH RamDiskDevicePath;
extern RAM_DISK RamDisk;

#endif /* #if (defined(__RAM_DISK_C)) */


#endif /* #ifndef __EFI_RAM_DISK_H__ */
