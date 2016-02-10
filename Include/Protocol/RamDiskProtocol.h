/** @file
  TODO: Brief Description of Protocol RamDiskProtocol
  
  TODO: Detailed Description of Protocol RamDiskProtocol

  TODO: Copyright for Protocol RamDiskProtocol
  
  TODO: License for Protocol RamDiskProtocol
  
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
