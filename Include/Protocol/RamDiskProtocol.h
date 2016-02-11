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

#ifndef __RAM_DISK_PROTOCOL_H__
#define __RAM_DISK_PROTOCOL_H__

#define EFI_RAM_DISK_PROTOCOL_PROTOCOL_GUID \
  {0x8eb35670, 0xd035, 0x11e5, {0xa6, 0x34, 0x00, 0x50, 0x56, 0xc0, 0x00, 0x08}}

///
/// Forward declaration
///  
typedef struct _EFI_RAM_DISK_PROTOCOL_PROTOCOL EFI_RAM_DISK_PROTOCOL_PROTOCOL;

///
/// Function prototypes
///
typedef
EFI_STATUS
(EFIAPI *EFI_RAM_DISK_PROTOCOL_FUNCTION_NAME_1)(
  IN EFI_RAM_DISK_PROTOCOL_PROTOCOL  *This
  //
  // Place additional function arguments here
  //
  );

typedef
EFI_STATUS
(EFIAPI *EFI_RAM_DISK_PROTOCOL_FUNCTION_NAME_2)(
  IN EFI_RAM_DISK_PROTOCOL_PROTOCOL  *This
  //
  // Place additional function arguments here
  //
  );

typedef
EFI_STATUS
(EFIAPI *EFI_RAM_DISK_PROTOCOL_FUNCTION_NAME_N)(
  IN EFI_RAM_DISK_PROTOCOL_PROTOCOL  *This
  //
  // Place additional function arguments here
  //
  );

///
/// Protocol structure
///
typedef struct _EFI_RAM_DISK_PROTOCOL_PROTOCOL {
  EFI_RAM_DISK_PROTOCOL_FUNCTION_NAME_1  Function1;
  EFI_RAM_DISK_PROTOCOL_FUNCTION_NAME_2  Function2;
  // . . .
  EFI_RAM_DISK_PROTOCOL_FUNCTION_NAME_N  FunctionN;
  //
  // Place protocol data fields here
  //
}

extern EFI_GUID gEfiRamDiskProtocolProtocolGuid;

#endif
