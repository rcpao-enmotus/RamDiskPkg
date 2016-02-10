/* ---------------------------------------------------------------------------
pr.h
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


extern UINTN PrIndentLevel;


void PrIndent(void);
void PrBufxxdr(void *pBuf, UINT32 Length);
void PrCapacityDec(UINT64 capacity);
