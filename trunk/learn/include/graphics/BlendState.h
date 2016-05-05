#pragma once
#include <math/algebra/Vector4D.h>
#include <graphics/DrawingState.h>

namespace plu{
	class BlendState : public DrawingState{
	public:
		/*下列各种枚举的定义都是对dx11的一些枚举的对应*/
		/*定义混合状态的各种方式*/
		enum Mode{
			BM_ZERO,
			BM_ONE,
			BM_SRC_COLOR,
			BM_INV_SRC_COLOR,
			BM_SRC_ALPHA,
			BM_INV_SRC_ALPHA,
			BM_DEST_ALPHA,
			BM_INV_DEST_ALPHA,
			BM_DEST_COLOR,
			BM_INV_DEST_COLOR,
			BM_SRC_ALPHA_SAT,
			BM_FACTOR,
			BM_INV_FACTOR,
			BM_SRC1_COLOR,
			BM_INV_SRC1_COLOR,
			BM_SRC1_ALPHA,
			BM_INV_SRC1_ALPHA
		};

		/*混合状态的操作方式*/
		enum Operation{
			OP_ADD,
			OP_SUBTRACT,
			OP_REV_SUBTRACT,
			OP_MIN,
			OP_MAX
		};

		enum ColorWrite{
			CW_ENABLE_RED = 1,
			CW_ENABLE_GREEN = 2,
			CW_ENABLE_BLUE = 4,
			CW_ENABLE_ALPHA = 8,
			CW_ENABLE_ALL = 15
		};

		enum{
			NUM_TARGETS = 8
		};

		struct Target{
			bool enable;        // default: false
			Mode srcColor;      // default: BM_ONE
			Mode dstColor;      // default: BM_ZERO
			Operation opColor;  // default: OP_ADD
			Mode srcAlpha;      // default: BM_ONE
			Mode dstAlpha;      // default: BM_ZERO
			Operation opAlpha;  // default: OP_ADD
			unsigned char mask; // default: CW_ENABLE_ALL
		};


		BlendState();
		~BlendState();

		bool enableAlphaToCoverage;     // default: false
		bool enableIndependentBlend;    // default: false
		Target target[NUM_TARGETS];
		unsigned int sampleMask;        // default: 0xFFFFFFFF, D3D11_DEFAULT_SAMPLE_MASK
	};
}