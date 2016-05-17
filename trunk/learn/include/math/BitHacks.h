#pragma once
#include <cstdint>

// Convenient macros for manipulating 64-bit integers.
#define GTE_I64(v) v##LL
#define GTE_U64(v) v##ULL
#define GTE_GET_LO_I64(v) (int32_t)((v) & 0x00000000ffffffffLL)
#define GTE_GET_HI_I64(v) (int32_t)(((v) >> 32) & 0x00000000ffffffffLL)
#define GTE_GET_LO_U64(v) (uint32_t)((v) & 0x00000000ffffffffULL)
#define GTE_GET_HI_U64(v) (uint32_t)(((v) >> 32) & 0x00000000ffffffffULL)
#define GTE_SET_LO_I64(v,lo) (((v) & 0xffffffff00000000LL) | (int64_t)(lo))
#define GTE_SET_HI_I64(v,hi) (((v) & 0x00000000ffffffffLL) | ((int64_t)(hi) << 32))
#define GTE_MAKE_I64(hi,lo)  ((int64_t)(lo) | ((int64_t)(hi) << 32))
#define GTE_SET_LO_U64(v,lo) (((v) & 0xffffffff00000000ULL) | (uint64_t)(lo))
#define GTE_SET_HI_U64(v,hi) (((v) & 0x00000000ffffffffULL) | ((uint64_t)(hi) << 32))
#define GTE_MAKE_U64(hi,lo)  ((uint64_t)(lo) | ((uint64_t)(hi) << 32))

namespace plu{
	bool IsPowerOfTwo(int value);
	int Log2OfPowerOfTwo(int powerOfTwo);

	int GetLeadingBit(int value);
	int GetTrailingBit(int value);

	// Round up to a power of two.  If input is zero, the return is 1.  If input
	// is larger than 2^{31}, the return is 2^{32}.
	int RoundUpToPowerOfTwo(int value);

	// Round down to a power of two.  If input is zero, the return is 0.
	int RoundDownToPowerOfTwo(int value);

}
