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

/* ---------------------------------------------------------------------------
pr.c
--------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------
Include Files
--------------------------------------------------------------------------- */
#include <Library/UefiLib.h> /* Print() */
#include <Library/BaseLib/BaseLibInternals.h> /* InternalMathDivU64x32 */

#define __PR_C
#include "pr.h"


/* ---------------------------------------------------------------------------
Defines
--------------------------------------------------------------------------- */

//#include <ctype.h> /* isprint */
#define isprint(c) ((c > 0x1F) && (c != 0x7F))


/* ---------------------------------------------------------------------------
Functions
--------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------
PrIndent
	Print PrIndentLevel spaces.
Input
	PrIndentLevel
Output
	nothing
Returns
	nothing
--------------------------------------------------------------------------- */
void PrIndent(void) {
	UINTN i;

	for (i = 0; i < PrIndentLevel; i++) {
		AsciiPrint(" ");
	}
} /* PrIndent */


/* ---------------------------------------------------------------------------
;PrBufxxdr
;  Print buffer using 'xxd -r' output format.
;$ xxd -g 1 input.bin
;0000000: fa 31 c0 8e d8 8e c0 8e-d0 bc 00 7c 89 e6 bf 00  .1.........|....
;0000010: 06 b9 00 01 f3 a5 ea 1b 06 00 00 6a 02 9d be c6  ...........j....
;0000020: 06 e8 75 00 be f6 06 e8 6f 00 e8 93 00 be 19 07  ..u.....o.......
;0000030: e8 66 00 be 1c 07 e8 60 00 52 b4 41 bb aa 55 cd  .f.....`.R.A..U.
;0000040: 13 5a 72 35 be 2c 07 e8 4f 00 be 8a 07 c7 44 04  .Zr5.,..O.....D.
;0000050: 00 08 b4 42 cd 13 72 29 be 2c 07 e8 3b 00 be 8a  ...B..r).,..;...
;0000060: 07 80 7c 02 27 75 22 be 2c 07 e8 2c 00 be f6 06  ..|.'u".,..,....
;0000070: e8 26 00 e8 4a 00 e9 87 01 be 2f 07 e8 1a 00 eb  .&..J...../.....
;0000080: 10 be 40 07 e8 12 00 eb 08 be 52 07 e8 0a 00 eb  ..@.......R.....
;0000090: 00 be 63 07 e8 02 00 eb fe 50 53 56 2e 8a 04 08  ..c......PSV....
;00000a0: c0 74 0a bb 01 00 b4 0e cd 10 46 eb ef 5e 5b 58  .t........F..^[X
;00000b0: c3 50 b4 01 cd 16 58 74 06 9c 30 e4 cd 16 9d c3  .P....Xt..0.....
;00000c0: e8 ee ff 74 fb c3 65 6e 5f 62 6f 6f 74 69 65 72  ...t..en_bootier
;00000d0: 20 6d 62 72 30 36 30 30 2d 61 73 61 70 2e 6e 61   mbr0600-asap.na
;00000e0: 73 6d 20 56 65 72 73 69 6f 6e 20 30 2e 30 2e 33  sm Version 0.0.3
;00000f0: 38 35 33 0d 0a 00 0d 0a 50 72 65 73 73 20 61 6e  853.....Press an
;0000100: 79 20 6b 65 79 20 74 6f 20 63 6f 6e 74 69 6e 75  y key to continu
;0000110: 65 20 2e 20 2e 20 2e 20 00 0d 0a 00 4c 6f 61 64  e . . . ....Load
;0000120: 69 6e 67 20 73 74 61 67 65 20 31 00 20 2e 00 0d  ing stage 1. ...
;0000130: 0a 4d 73 67 4e 6f 45 78 74 52 65 61 64 0d 0a 00  .MsgNoExtRead...
;Offset-  Hex--------------------------------------------  Printable-------
;
;To convert back to binary:
;$ xxd -r input.txt output.bin
;Warning: xdd -r may only allow 7 hex digits for the offset.
;
;Ported from en_bootier/common-asap.nasm.
--------------------------------------------------------------------------- */
void PrBufxxdr(void *pBuf, UINTN Length) {
  unsigned char *p = pBuf;
  UINTN Offs = 0;
  int BytesInLinePrinted = 0;
  int PrintablesPrinted = 0;


  /* Print pBuf pointer value */
  Print(L"%p for %08x\n", pBuf, Length);

  do {

    /* Print "Offs:" */
    Print(L"%08x:", Offs);

    /* Print " 00 01 02 03 04 05 06 07-08 09 0A 0B 0C 0D 0E 0F" */
    for (BytesInLinePrinted = 0; (BytesInLinePrinted < 16) && (Length); BytesInLinePrinted++) {
      Print(L"%c", (BytesInLinePrinted == 8)? '-': ' ');
      Print(L"%02x", *p);
      p++;
      Offs++;
      Length--;
    }

    /* Pad hex line to Printables column */
    for (PrintablesPrinted = BytesInLinePrinted; PrintablesPrinted < 16; PrintablesPrinted++) {
      Print(L"   ");
    }

    /* Print Printables line */
    p -= BytesInLinePrinted; /* Rewind for Printables */
    Print(L" ");
    for (PrintablesPrinted = 0; PrintablesPrinted < BytesInLinePrinted; PrintablesPrinted++) {
      Print(L"%c", (isprint(*p))? *p: '.');
      p++;
    }

    Print(L"\n");
  } while (Length);

} /* PrBufxxdr */


/* ---------------------------------------------------------------------------
PrCapacityDec
--------------------------------------------------------------------------- */
void PrCapacityDec(UINT64 capacity) {
  UINT32 capUnit;
  UINT16 *capUnits[] = {
	L"B",
	L"KB",
	L"MB",
	L"GB",
	L"TB",
	L"PB",
	L"EB",
	L"ZB"
  };


  capUnit = 0; /* B */
  if(capacity > 999LL) {
	capUnit++; /* KB */
	capacity = InternalMathDivU64x32(capacity, 1000);
  }
  if (capacity > 999LL) {
	capUnit++; /* MB */
	capacity = InternalMathDivU64x32(capacity, 1000);
  }
  if (capacity > 999LL) {
	capUnit++; /* GB */
	capacity = InternalMathDivU64x32(capacity, 1000);
  }
  if (capacity > 999LL) {
	capUnit++; /* TB */
	capacity = InternalMathDivU64x32(capacity, 1000);
  }
  if (capacity > 999LL) {
	capUnit++; /* PB */
	capacity = InternalMathDivU64x32(capacity, 1000);
  }
  if (capacity > 999LL) {
	capUnit++; /* EB */
	capacity = InternalMathDivU64x32(capacity, 1000);
  }
  if (capacity > 999LL) {
	capUnit++; /* ZB */
	capacity = InternalMathDivU64x32(capacity, 1000);
  }

  Print(L"%lld %s", capacity, capUnits[capUnit]);
} /* PrCapacityDec */

