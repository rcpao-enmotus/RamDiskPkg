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

/* ---------------------------------------------------------------------------
pr.h
--------------------------------------------------------------------------- */


#ifndef __PR_H__
#define __PR_H__


/* ---------------------------------------------------------------------------
Defines
--------------------------------------------------------------------------- */

/* %u does not work in AsciiPrint() */
#define PRId8 "d"
//#define PRIu8 "u"
#define PRIx8 "x"
#define PRIX8 "X"
#define PRId16 "d"
//#define PRIu16 "u"
#define PRIx16 "x"
#define PRIX16 "X"
#define PRId32 "d"
//#define PRIu32 "u"
#define PRIx32 "x"
#define PRIX32 "X"
/* %I64d */
#define PRId64 "ld"
//#define PRIu64 "lu"
#define PRIx64 "lx"
#define PRIX64 "lX"


#define PR(fmt, ...) \
	do { \
		AsciiPrint(fmt, __VA_ARGS__); \
	} while (0)


/* http://stackoverflow.com/questions/1644868/c-define-macro-for-debug-printing */

#define DL_00 0x00
#define DL_01 0x01
#define DL_0A 0x0A
#define DL_80 0x80
#define DL_FE 0xFE
#define DL_FF 0xFF
#define DL_MIN DL_01
#define DL_MAX DL_FE
#define DL_DISABLED DL_FF
#define DEFAULT_DBG_LEVEL DL_MAX

#define DBG_ENABLED 1

#define DBG_X(dbglvl, x) \
	do { \
		if ((DBG_ENABLED) && (DbgLevel >= dbglvl)) { \
			x; \
		} \
	} while (0)

#define DBG_PR(dbglvl, fmt, ...) \
	do { \
		if ((DBG_ENABLED) && (DbgLevel >= dbglvl)) { \
			AsciiPrint("%a(%d): ", FN, __LINE__); \
			AsciiPrint(fmt, __VA_ARGS__); \
		} \
	} while (0)

#define DBG_PR1(dbglvl, fn, ln, fmt, ...) \
	do { \
		if ((DBG_ENABLED) && (DbgLevel >= dbglvl)) { \
			AsciiPrint("%a(%d): ", fn, ln); \
			AsciiPrint(fmt, __VA_ARGS__); \
		} \
	} while (0)

/* Example use: DBG_PR2((fmt, ...)) */
#define DBG_PR2(args) \
	do { \
		AsciiPrint(args); \
	} while (0)


#define DBG_PR_BUF(dbglvl, pBuf, Length) \
	do { \
		if ((DBG_ENABLED) && (DbgLevel >= dbglvl)) { \
			PrBufxxdr(pBuf, Length); \
		} \
	} while (0)


/* ---------------------------------------------------------------------------
Externals
--------------------------------------------------------------------------- */

#if (defined(__PR_C))

UINTN DbgLevel = DEFAULT_DBG_LEVEL;
UINTN PrIndentLevel = 0;

#else /* #if (defined(__PR_C)) */

extern UINTN DbgLevel;
extern UINTN PrIndentLevel;

#endif /* #if (defined(__PR_C)) */


/* ---------------------------------------------------------------------------
Function Prototypes
--------------------------------------------------------------------------- */

void PrIndent(void);
void PrBufxxdr(void *pBuf, UINTN Length);
void PrCapacityDec(UINT64 capacity);

#endif /* #ifndef __PR_H__ */
