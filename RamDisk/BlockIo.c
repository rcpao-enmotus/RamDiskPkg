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

#include "RamDisk.h"

///
/// Block I/O Media structure
///
GLOBAL_REMOVE_IF_UNREFERENCED 
EFI_BLOCK_IO_MEDIA  gRamDiskBlockIoMedia = {
  0,      // MediaId
  FALSE,  // RemovableMedia
  FALSE,  // MediaPresent
  TRUE,   // LogicalPartition
  FALSE,  // ReadOnly
  FALSE,  // WriteCaching
  512,    // BlockSize
  0,      // IoAlign
  0,      // LastBlock
  0,      // LowestAlignedLba
  0,      // LogicalBlocksPerPhysicalBlock
  0       // OptimalTransferLengthGranularity
};

///
/// Block I/O Protocol instance
///
GLOBAL_REMOVE_IF_UNREFERENCED 
EFI_BLOCK_IO_PROTOCOL  gRamDiskBlockIo = {
  EFI_BLOCK_IO_PROTOCOL_REVISION3,             // Revision
  &gRamDiskBlockIoMedia,                // Media
  (EFI_BLOCK_RESET)RamDiskBlockIoReset, // Reset
  RamDiskBlockIoReadBlocks,             // ReadBlocks
  RamDiskBlockIoWriteBlocks,            // WriteBlocks
  RamDiskBlockIoFlushBlocks             // FlushBlocks
};

///
/// Block I/O 2 Protocol instance
///
GLOBAL_REMOVE_IF_UNREFERENCED 
EFI_BLOCK_IO2_PROTOCOL  gRamDiskBlockIo2 = {
  &gRamDiskBlockIoMedia,        // Media
  RamDiskBlockIoResetEx,        // Reset
  RamDiskBlockIoReadBlocksEx,   // ReadBlocks
  RamDiskBlockIoWriteBlocksEx,  // WriteBlocks
  RamDiskBlockIoFlushBlocksEx   // FlushBlocks
};


/**
  Reset the block device hardware.

  @param[in]  This                 Indicates a pointer to the calling context.
  @param[in]  ExtendedVerification Indicates that the driver may perform a more
                                   exhausive verfication operation of the device
                                   during reset.

  @retval EFI_SUCCESS          The device was reset.
  @retval EFI_DEVICE_ERROR     The device is not functioning properly and could
                               not be reset.

**/
EFI_STATUS
EFIAPI
RamDiskBlockIoReset (
  IN EFI_BLOCK_IO_PROTOCOL  *This,
  IN BOOLEAN                 ExtendedVerification
  )
{
  return EFI_SUCCESS;
}

/**
  Read BufferSize bytes from Lba into Buffer.

  @param  This       Indicates a pointer to the calling context.
  @param  MediaId    Id of the media, changes every time the media is replaced.
  @param  Lba        The starting Logical Block Address to read from
  @param  BufferSize Size of Buffer, must be a multiple of device block size.
  @param  Buffer     A pointer to the destination buffer for the data. The caller is
                     responsible for either having implicit or explicit ownership of the buffer.

  @retval EFI_SUCCESS           The data was read correctly from the device.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing the read.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId does not matched the current device.
  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.
  @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.

**/
EFI_STATUS
EFIAPI
RamDiskBlockIoReadBlocks (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  OUT VOID                          *Buffer
  )
{
#undef FN
#define FN "RamDiskBlockIoReadBlocks"
#define DBG_RamDiskBlockIoReadBlocks DL_DISABLED /* DL_DISABLED DL_80 */
  EFI_BLOCK_IO_MEDIA *Media;
  RAM_DISK *RamDisk;
  EFI_PHYSICAL_ADDRESS RamBufferPtr;


  DBG_PR(DBG_RamDiskBlockIoReadBlocks, "MediaId=%08X LBA=%016lX BufferSize=%d Buffer=%16lX\n", MediaId, Lba, BufferSize, Buffer);

  RamDisk = (RAM_DISK *)This;
  Media = &RamDisk->BlockIoMedia;

  if (BufferSize % Media->BlockSize != 0) {
    DBG_PR(DBG_RamDiskBlockIoReadBlocks, "Error: (BufferSize % Media->BlockSize != 0)\n");
    return EFI_BAD_BUFFER_SIZE;
  }

  if (Lba > Media->LastBlock) {
    DBG_PR(DBG_RamDiskBlockIoReadBlocks, "Error: (Lba > Media->LastBlock)\n");
    return EFI_INVALID_PARAMETER;
  }

  if (Lba + BufferSize / Media->BlockSize - 1 > Media->LastBlock) {
    DBG_PR(DBG_RamDiskBlockIoReadBlocks, "Error: (Lba + BufferSize / Media->BlockSize - 1 > Media->LastBlock)\n");
    return EFI_INVALID_PARAMETER;
  }

  RamBufferPtr = (EFI_PHYSICAL_ADDRESS)RamDisk->RamBuffer + MultU64x32(RamDisk->AddLbaOfs + Lba, Media->BlockSize);
  CopyMem(Buffer, (VOID*)RamBufferPtr, BufferSize);
  DBG_PR(DBG_RamDiskBlockIoReadBlocks, "Lba=%016lX bs=%08X RamBufferPtr=%016lX\n", Lba, Media->BlockSize, RamBufferPtr);
  //DBG_PR_BUF(DBG_RamDiskBlockIoReadBlocks, Buffer, BufferSize);

  return (EFI_SUCCESS);
}

/**
  Write BufferSize bytes from Lba into Buffer.

  @param  This       Indicates a pointer to the calling context.
  @param  MediaId    The media ID that the write request is for.
  @param  Lba        The starting logical block address to be written. The caller is
                     responsible for writing to only legitimate locations.
  @param  BufferSize Size of Buffer, must be a multiple of device block size.
  @param  Buffer     A pointer to the source buffer for the data.

  @retval EFI_SUCCESS           The data was written correctly to the device.
  @retval EFI_WRITE_PROTECTED   The device can not be written to.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing the write.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHNAGED     The MediaId does not matched the current device.
  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.
  @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.

**/
EFI_STATUS
EFIAPI
RamDiskBlockIoWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  IN VOID                           *Buffer
  )
{
#undef FN
#define FN "RamDiskBlockIoWriteBlocks"
#define DBG_RamDiskBlockIoWriteBlocks DL_DISABLED /* DL_DISABLED DL_80 */
  EFI_BLOCK_IO_MEDIA *Media;
  RAM_DISK *RamDisk;
  EFI_PHYSICAL_ADDRESS RamBufferPtr;


  DBG_PR(DBG_RamDiskBlockIoWriteBlocks, "MediaId=%08X LBA=%016lX BufferSize=%d Buffer=%16lX\n", MediaId, Lba, BufferSize, Buffer);

  RamDisk = (RAM_DISK *)This;
  Media = &RamDisk->BlockIoMedia;

  if (BufferSize % Media->BlockSize != 0) {
    DBG_PR(DBG_RamDiskBlockIoReadBlocks, "Error: (BufferSize % Media->BlockSize != 0)\n");
    return EFI_BAD_BUFFER_SIZE;
  }

  if (Lba > Media->LastBlock) {
    DBG_PR(DBG_RamDiskBlockIoReadBlocks, "Error: (Lba > Media->LastBlock)\n");
    return EFI_INVALID_PARAMETER;
  }

  if (Lba + BufferSize / Media->BlockSize - 1 > Media->LastBlock) {
    DBG_PR(DBG_RamDiskBlockIoReadBlocks, "Error: (Lba + BufferSize / Media->BlockSize - 1 > Media->LastBlock)\n");
    return EFI_INVALID_PARAMETER;
  }

  RamBufferPtr = (EFI_PHYSICAL_ADDRESS)RamDisk->RamBuffer + MultU64x32(RamDisk->AddLbaOfs + Lba, Media->BlockSize);
  CopyMem((VOID*)RamBufferPtr, Buffer, BufferSize);
  DBG_PR(DBG_RamDiskBlockIoWriteBlocks, "Lba=%016lX bs=%08X RamBufferPtr=%016lX\n", Lba, Media->BlockSize, RamBufferPtr);
  //DBG_PR_BUF(DBG_RamDiskBlockIoWriteBlocks, Buffer, BufferSize);

  return (EFI_SUCCESS);
}

/**
  Flush the Block Device.

  @param  This              Indicates a pointer to the calling context.

  @retval EFI_SUCCESS       All outstanding data was written to the device
  @retval EFI_DEVICE_ERROR  The device reported an error while writting back the data
  @retval EFI_NO_MEDIA      There is no media in the device.

**/
EFI_STATUS
EFIAPI
RamDiskBlockIoFlushBlocks (
  IN EFI_BLOCK_IO_PROTOCOL  *This
  )
{
  return EFI_SUCCESS;
}

/**
  Reset the block device hardware.

  @param[in]  This                 Indicates a pointer to the calling context.
  @param[in]  ExtendedVerification Indicates that the driver may perform a more
                                   exhausive verfication operation of the device
                                   during reset.

  @retval EFI_SUCCESS          The device was reset.
  @retval EFI_DEVICE_ERROR     The device is not functioning properly and could
                               not be reset.

**/
EFI_STATUS
EFIAPI
RamDiskBlockIoResetEx (
  IN EFI_BLOCK_IO2_PROTOCOL  *This,
  IN BOOLEAN                 ExtendedVerification
  )
{
  return EFI_SUCCESS;
}

/**
  Read BufferSize bytes from Lba into Buffer.
  
  This function reads the requested number of blocks from the device. All the
  blocks are read, or an error is returned.
  If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_or EFI_MEDIA_CHANGED is returned and
  non-blocking I/O is being used, the Event associated with this request will
  not be signaled.

  @param[in]       This       Indicates a pointer to the calling context.
  @param[in]       MediaId    Id of the media, changes every time the media is 
                              replaced.
  @param[in]       Lba        The starting Logical Block Address to read from.
  @param[in, out]  Token	    A pointer to the token associated with the transaction.
  @param[in]       BufferSize Size of Buffer, must be a multiple of device block size.  
  @param[out]      Buffer     A pointer to the destination buffer for the data. The 
                              caller is responsible for either having implicit or 
                              explicit ownership of the buffer.

  @retval EFI_SUCCESS           The read request was queued if Token->Event is
                                not NULL.The data was read correctly from the
                                device if the Token->Event is NULL.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing
                                the read.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId is not for the current media.
  @retval EFI_BAD_BUFFER_SIZE   The BufferSize parameter is not a multiple of the
                                intrinsic block size of the device.
  @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack
                                of resources.
**/
EFI_STATUS
EFIAPI
RamDiskBlockIoReadBlocksEx (
  IN     EFI_BLOCK_IO2_PROTOCOL *This,
  IN     UINT32                 MediaId,
  IN     EFI_LBA                LBA,
  IN OUT EFI_BLOCK_IO2_TOKEN    *Token,
  IN     UINTN                  BufferSize,
  OUT    VOID                  *Buffer
  )
{
  return EFI_DEVICE_ERROR;
}

/**
  Write BufferSize bytes from Lba into Buffer.

  This function writes the requested number of blocks to the device. All blocks
  are written, or an error is returned.If EFI_DEVICE_ERROR, EFI_NO_MEDIA,
  EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED is returned and non-blocking I/O is
  being used, the Event associated with this request will not be signaled.

  @param[in]       This       Indicates a pointer to the calling context.
  @param[in]       MediaId    The media ID that the write request is for.
  @param[in]       Lba        The starting logical block address to be written. The
                              caller is responsible for writing to only legitimate
                              locations.
  @param[in, out]  Token      A pointer to the token associated with the transaction.
  @param[in]       BufferSize Size of Buffer, must be a multiple of device block size.
  @param[in]       Buffer     A pointer to the source buffer for the data.

  @retval EFI_SUCCESS           The write request was queued if Event is not NULL.
                                The data was written correctly to the device if
                                the Event is NULL.
  @retval EFI_WRITE_PROTECTED   The device can not be written to.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHNAGED     The MediaId does not matched the current device.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing the write.
  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.
  @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack
                                of resources.

**/
EFI_STATUS
EFIAPI
RamDiskBlockIoWriteBlocksEx (
  IN     EFI_BLOCK_IO2_PROTOCOL  *This,
  IN     UINT32                 MediaId,
  IN     EFI_LBA                LBA,
  IN OUT EFI_BLOCK_IO2_TOKEN    *Token,
  IN     UINTN                  BufferSize,
  IN     VOID                   *Buffer
  )
{
  return EFI_DEVICE_ERROR;
}

/**
  Flush the Block Device.
 
  If EFI_DEVICE_ERROR, EFI_NO_MEDIA,_EFI_WRITE_PROTECTED or EFI_MEDIA_CHANGED
  is returned and non-blocking I/O is being used, the Event associated with
  this request will not be signaled.  

  @param[in]      This     Indicates a pointer to the calling context.
  @param[in,out]  Token    A pointer to the token associated with the transaction

  @retval EFI_SUCCESS          The flush request was queued if Event is not NULL.
                               All outstanding data was written correctly to the
                               device if the Event is NULL.
  @retval EFI_DEVICE_ERROR     The device reported an error while writting back
                               the data.
  @retval EFI_WRITE_PROTECTED  The device cannot be written to.
  @retval EFI_NO_MEDIA         There is no media in the device.
  @retval EFI_MEDIA_CHANGED    The MediaId is not for the current media.
  @retval EFI_OUT_OF_RESOURCES The request could not be completed due to a lack
                               of resources.

**/
EFI_STATUS
EFIAPI
RamDiskBlockIoFlushBlocksEx (
  IN     EFI_BLOCK_IO2_PROTOCOL   *This,
  IN OUT EFI_BLOCK_IO2_TOKEN      *Token
  )
{
  return EFI_DEVICE_ERROR;
}
