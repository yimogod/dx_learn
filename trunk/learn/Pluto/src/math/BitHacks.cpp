#include <math/BitHacks.h>

namespace plu{

	static int const gsLeadingBitTable[32] = {
		 0,  9,  1, 10, 13, 21,  2, 29,
		11, 14, 16, 18, 22, 25,  3, 30,
		 8, 12, 20, 28, 15, 17, 24,  7,
		19, 27, 23,  6, 26,  5,  4, 31
	};

	static int const gsTrailingBitTable[32] = {
		 0,  1, 28,  2, 29, 14, 24,  3,
		30, 22, 20, 15, 25, 17,  4,  8, 
		31, 27, 13, 23, 21, 19, 16,  7,
		26, 12, 18,  6, 11,  5, 10,  9
	};


	bool IsPowerOfTwo(int value){
		return (value > 0) && ((value & (value - 1)) == 0);
	}

	int Log2OfPowerOfTwo(int powerOfTwo){
		uint32_t log2 = (powerOfTwo & 0xAAAAAAAAu) != 0;
		log2 |= ((powerOfTwo & 0xFFFF0000u) != 0) << 4;
		log2 |= ((powerOfTwo & 0xFF00FF00u) != 0) << 3;
		log2 |= ((powerOfTwo & 0xF0F0F0F0u) != 0) << 2;
		log2 |= ((powerOfTwo & 0xCCCCCCCCu) != 0) << 1;
		return log2;
	}

	int GetLeadingBit(int value){
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		uint32_t key = (value * 0x07C4ACDDu) >> 27;
		return gsLeadingBitTable[key];
	}

	int GetTrailingBit(int value){
		int key = ((int)((value & -value) * 0x077CB531u)) >> 27;
		return gsTrailingBitTable[key];
	}

	int RoundUpToPowerOfTwo(int value){
		if (value > 0){
			int leading = GetLeadingBit(value);
			int mask = (1 << leading);
			if ((value & ~mask) == 0){
				return value;
			}else{
				// round up to a power of two
				return mask << 1;
			}
		}

		return 1;
	}

	int RoundDownToPowerOfTwo(int value){
		if (value > 0){
			int leading = GetLeadingBit(value);
			int mask = (1 << leading);
			return mask;
		}
		return 0;
	}

}
