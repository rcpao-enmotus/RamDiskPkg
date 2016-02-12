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

#define __RAM_DISK_C
#include "RamDisk.h"

///
/// Driver Support EFI Version Protocol instance
///
GLOBAL_REMOVE_IF_UNREFERENCED 
EFI_DRIVER_SUPPORTED_EFI_VERSION_PROTOCOL gRamDiskDriverSupportedEfiVersion = {
  sizeof (EFI_DRIVER_SUPPORTED_EFI_VERSION_PROTOCOL),
  0x0002001E
};

///
/// Driver Binding Protocol instance
///
EFI_DRIVER_BINDING_PROTOCOL gRamDiskDriverBinding = {
  RamDiskDriverBindingSupported,
  RamDiskDriverBindingStart,
  RamDiskDriverBindingStop,
  RAM_DISK_VERSION,
  NULL,
  NULL
};

/**
  Unloads an image.

  @param  ImageHandle           Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
  @retval EFI_INVALID_PARAMETER ImageHandle is not a valid image handle.

**/
EFI_STATUS 
EFIAPI
RamDiskUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  *HandleBuffer;
  UINTN       HandleCount;
  UINTN       Index;

  Status = EFI_SUCCESS;
  //
  // Retrieve array of all handles in the handle database
  //
  Status = gBS->LocateHandleBuffer (
                  AllHandles,
                  NULL,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Disconnect the current driver from handles in the handle database 
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->DisconnectController (HandleBuffer[Index], gImageHandle, NULL);
  }

  //
  // Free the array of handles
  //
  FreePool (HandleBuffer);

  //
  // Uninstall protocols installed in the driver entry point
  // 
  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ImageHandle,
                  &gEfiDriverBindingProtocolGuid,  &gRamDiskDriverBinding,
                  &gEfiComponentNameProtocolGuid,  &gRamDiskComponentName,
                  &gEfiComponentName2ProtocolGuid, &gRamDiskComponentName2,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Uninstall Driver Supported EFI Version Protocol onto ImageHandle
  //
  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ImageHandle,
                  &gEfiDriverSupportedEfiVersionProtocolGuid, &gRamDiskDriverSupportedEfiVersion,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Do any additional cleanup that is required for this driver
  //

  return EFI_SUCCESS;
}

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
RamDiskDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  //
  // Install UEFI Driver Model protocol(s).
  //
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gRamDiskDriverBinding,
             ImageHandle,
             &gRamDiskComponentName,
             &gRamDiskComponentName2
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Install Driver Supported EFI Version Protocol onto ImageHandle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiDriverSupportedEfiVersionProtocolGuid, &gRamDiskDriverSupportedEfiVersion,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Tests to see if this driver supports a given controller. If a child device is provided, 
  it further tests to see if this driver supports creating a handle for the specified child device.

  This function checks to see if the driver specified by This supports the device specified by 
  ControllerHandle. Drivers will typically use the device path attached to 
  ControllerHandle and/or the services from the bus I/O abstraction attached to 
  ControllerHandle to determine if the driver supports ControllerHandle. This function 
  may be called many times during platform initialization. In order to reduce boot times, the tests 
  performed by this function must be very small, and take as little time as possible to execute. This 
  function must not change the state of any hardware devices, and this function must be aware that the 
  device specified by ControllerHandle may already be managed by the same driver or a 
  different driver. This function must match its calls to AllocatePages() with FreePages(), 
  AllocatePool() with FreePool(), and OpenProtocol() with CloseProtocol().  
  Because ControllerHandle may have been previously started by the same driver, if a protocol is 
  already in the opened state, then it must not be closed with CloseProtocol(). This is required 
  to guarantee the state of ControllerHandle is not modified by this function.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to test. This handle 
                                   must support a protocol interface that supplies 
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This 
                                   parameter is ignored by device drivers, and is optional for bus 
                                   drivers. For bus drivers, if this parameter is not NULL, then 
                                   the bus driver must determine if the bus controller specified 
                                   by ControllerHandle and the child controller specified 
                                   by RemainingDevicePath are both supported by this 
                                   bus driver.

  @retval EFI_SUCCESS              The device specified by ControllerHandle and
                                   RemainingDevicePath is supported by the driver specified by This.
  @retval EFI_ALREADY_STARTED      The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by the driver
                                   specified by This.
  @retval EFI_ACCESS_DENIED        The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by a different
                                   driver or an application that requires exclusive access.
                                   Currently not implemented.
  @retval EFI_UNSUPPORTED          The device specified by ControllerHandle and
                                   RemainingDevicePath is not supported by the driver specified by This.
**/
EFI_STATUS
EFIAPI
RamDiskDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
#undef FN
#define FN "RamDiskDriverBindingSupported"
#define DBG_RamDiskDriverBindingSupported DL_DISABLED /* DL_DISABLED DL_80 */
#if 1
  return (EFI_SUCCESS);
#else
  EFI_BLOCK_IO_PROTOCOL *BlockIoProtocol;
  EFI_STATUS EfiStatus = EFI_SUCCESS;


  DBG_PR(DBG_RamDiskDriverBindingSupported, "entered\n");


  /* gEfiDevicePathProtocolGuid */

  EfiStatus = gBS->OpenProtocol (
    ControllerHandle,
    &gEfiDevicePathProtocolGuid,
    NULL,
    This->DriverBindingHandle,
    ControllerHandle,
    EFI_OPEN_PROTOCOL_TEST_PROTOCOL
  );
  if (EFI_ERROR(EfiStatus)) {
    DBG_PR(DBG_RamDiskDriverBindingSupported, "Error testing gEfiDevicePathProtocolGuid %r\n", EfiStatus);
    return (EfiStatus);
  }

  EfiStatus = gBS->CloseProtocol (
    ControllerHandle,
    &gEfiDevicePathProtocolGuid,
    This->DriverBindingHandle,
    ControllerHandle
  );


  /* gEfiBlockIoProtocolGuid */

  EfiStatus = gBS->OpenProtocol (
    ControllerHandle,
    &gEfiBlockIoProtocolGuid,
    (VOID **)&BlockIoProtocol,
    This->DriverBindingHandle,
    ControllerHandle,
    EFI_OPEN_PROTOCOL_TEST_PROTOCOL
  );
  if (EFI_ERROR(EfiStatus)) {
    DBG_PR(DBG_RamDiskDriverBindingSupported, "Error testing gEfiBlockIoProtocolGuid %r\n", EfiStatus);
    return (EfiStatus);
  }

  EfiStatus = gBS->CloseProtocol (
    ControllerHandle,
    &gEfiBlockIoProtocolGuid,
    This->DriverBindingHandle,
    ControllerHandle
  );


  DBG_PR(DBG_RamDiskDriverBindingSupported, "leaving\n");
  return (EfiStatus);
#endif
}

/**
  Starts a device controller or a bus controller.

  The Start() function is designed to be invoked from the EFI boot service ConnectController().
  As a result, much of the error checking on the parameters to Start() has been moved into this 
  common boot service. It is legal to call Start() from other locations, 
  but the following calling restrictions must be followed, or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE.
  2. If RemainingDevicePath is not NULL, then it must be a pointer to a naturally aligned
     EFI_DEVICE_PATH_PROTOCOL.
  3. Prior to calling Start(), the Supported() function for the driver specified by This must
     have been called with the same calling parameters, and Supported() must have returned EFI_SUCCESS.  

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This handle 
                                   must support a protocol interface that supplies 
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This 
                                   parameter is ignored by device drivers, and is optional for bus 
                                   drivers. For a bus driver, if this parameter is NULL, then handles 
                                   for all the children of Controller are created by this driver.  
                                   If this parameter is not NULL and the first Device Path Node is 
                                   not the End of Device Path Node, then only the handle for the 
                                   child device specified by the first Device Path Node of 
                                   RemainingDevicePath is created by this driver.
                                   If the first Device Path Node of RemainingDevicePath is 
                                   the End of Device Path Node, no child handle is created by this
                                   driver.

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.
  @retval Others                   The driver failded to start the device.

**/
EFI_STATUS
EFIAPI
RamDiskDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
#undef FN
#define FN "RamDiskDriverBindingStart"
#define DBG_RamDiskDriverBindingStart DL_DISABLED /* DL_DISABLED DL_80 */
  //EFI_BLOCK_IO_PROTOCOL *BlockIo;
  //EFI_BLOCK_IO2_PROTOCOL *BlockIo2;
  EFI_STATUS EfiStatus = EFI_UNSUPPORTED;


  DBG_PR(DBG_RamDiskDriverBindingStart, "entered\n");

  if (RamDisk.Installed) {
    return (EFI_ALREADY_STARTED);
  }

  if (RemainingDevicePath != NULL) {
    if (IsDevicePathEnd(RemainingDevicePath)) {
      EfiStatus = EFI_SUCCESS;
      DBG_PR(DBG_RamDiskDriverBindingStart, "RemainingDevicePath %x = END\n", RemainingDevicePath);
      return (EfiStatus);
    }
  }


  /* TBD Load the initial diskimage from a file instead of compiling the image into this driver. */
  RamDisk.RamBuffer = diskimage;
  RamDisk.RamBufferSize = diskimage_len;


  RamDisk.BlockIoProtocol.Revision       = EFI_BLOCK_IO_PROTOCOL_REVISION;
  RamDisk.BlockIoProtocol.Media          = &RamDisk.BlockIoMedia;
  RamDisk.BlockIoProtocol.ReadBlocks     = RamDiskBlockIoReadBlocks;
  RamDisk.BlockIoProtocol.WriteBlocks    = RamDiskBlockIoWriteBlocks;
  RamDisk.BlockIoProtocol.FlushBlocks    = RamDiskBlockIoFlushBlocks;
  RamDisk.BlockIoProtocol.Reset          = RamDiskBlockIoReset;

  RamDisk.BlockIoMedia.MediaId           = 0;
  RamDisk.BlockIoMedia.RemovableMedia    = FALSE;
  RamDisk.BlockIoMedia.MediaPresent      = TRUE;
  RamDisk.BlockIoMedia.LogicalPartition  = TRUE;
  RamDisk.BlockIoMedia.ReadOnly          = FALSE;
  RamDisk.BlockIoMedia.WriteCaching      = FALSE;
  RamDisk.BlockIoMedia.IoAlign           = 4;
  RamDisk.BlockIoMedia.BlockSize         = 512;
  RamDisk.BlockIoMedia.LastBlock         = RamDisk.RamBufferSize / RamDisk.BlockIoMedia.BlockSize - 1;

  {
    UINT64 DiskId;

    (void)gBS->GetNextMonotonicCount(&DiskId);
    DBG_PR(DBG_RamDiskDriverBindingStart, "GetNextMonotonicCount DiskId=%x\n", DiskId);

    CopyMem(&RamDiskDevicePath.DiskId, &DiskId, sizeof(DiskId));

    RamDisk.DevicePath = DuplicateDevicePath((EFI_DEVICE_PATH*)&RamDiskDevicePath);
    if(RamDisk.DevicePath == NULL) {
      DBG_PR(DBG_RamDiskDriverBindingStart, "DuplicateDevicePath returned NULL\n");
      EfiStatus = EFI_OUT_OF_RESOURCES;
      return (EfiStatus);
    }
  }


  EfiStatus = gBS->InstallMultipleProtocolInterfaces(
    &ControllerHandle,
    &gEfiBlockIoProtocolGuid,
    &RamDisk.BlockIoProtocol,

    &gEfiDevicePathProtocolGuid,
    RamDisk.DevicePath,

    NULL
  );
  if (EFI_ERROR(EfiStatus)) {
    DBG_PR(DBG_RamDiskDriverBindingStart, "InstallMultipleProtocolInterfaces failed %r\n", EfiStatus);
    return (EfiStatus);
  }


  RamDisk.Installed = TRUE;


  return EfiStatus;
}

/**
  Stops a device controller or a bus controller.
  
  The Stop() function is designed to be invoked from the EFI boot service DisconnectController(). 
  As a result, much of the error checking on the parameters to Stop() has been moved 
  into this common boot service. It is legal to call Stop() from other locations, 
  but the following calling restrictions must be followed, or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE that was used on a previous call to this
     same driver's Start() function.
  2. The first NumberOfChildren handles of ChildHandleBuffer must all be a valid
     EFI_HANDLE. In addition, all of these handles must have been created in this driver's
     Start() function, and the Start() function must have called OpenProtocol() on
     ControllerHandle with an Attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.
  
  @param[in]  This              A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle  A handle to the device being stopped. The handle must 
                                support a bus specific I/O protocol for the driver 
                                to use to stop the device.
  @param[in]  NumberOfChildren  The number of child device handles in ChildHandleBuffer.
  @param[in]  ChildHandleBuffer An array of child handles to be freed. May be NULL 
                                if NumberOfChildren is 0.

  @retval EFI_SUCCESS           The device was stopped.
  @retval EFI_DEVICE_ERROR      The device could not be stopped due to a device error.

**/
EFI_STATUS
EFIAPI
RamDiskDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  )
{
#undef FN
#define FN "RamDiskDriverBindingStop"
#define DBG_RamDiskDriverBindingStop DL_80 /* DL_DISABLED DL_80 */


  DBG_PR(DBG_RamDiskDriverBindingStop, "entered\n");
  return EFI_UNSUPPORTED;
}
