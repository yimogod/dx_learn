#pragma once
#include <graphics/DrawingState.h>

namespace plu{
	class DepthStencilState : public DrawingState{
	public:
		enum WriteMask{
			MASK_ZERO,
			MASK_ALL
		};

		enum Comparison{
			NEVER,
			LESS,
			EQUAL,
			LESS_EQUAL,
			GREATER,
			NOT_EQUAL,
			GREATER_EQUAL,
			ALWAYS
		};

		enum Operation{
			OP_KEEP,
			OP_ZERO,
			OP_REPLACE,
			OP_INCR_SAT,
			OP_DECR_SAT,
			OP_INVERT,
			OP_INCR,
			OP_DECR
		};

		struct Face{
			Operation fail;
			Operation pass;
			Operation depthFail;
			Comparison comparison;
		};

		DepthStencilState();
		~DepthStencilState();

		bool depthEnable;                   // default: true
		WriteMask writeMask;                // default: MASK_ALL
		Comparison comparison;              // default: LESS
		bool stencilEnable;                 // default: false
		unsigned char stencilReadMask;      // default: 0xFF
		unsigned char stencilWriteMask;     // default: 0xFF
		Face frontFace;                     // default: (KEEP,KEEP,KEEP,ALWAYS)
		Face backFace;                      // default: (KEEP,KEEP,KEEP,ALWAYS)
		unsigned int reference;             // default: 0, ÓÃÓÚOMSetDepthStencilStateµÄStencilRef
	};
}