#include <graphics/BlendState.h>
using namespace plu;

BlendState::BlendState(){
	enableAlphaToCoverage = false;
	enableIndependentBlend = false;
	blendColor = Vector4D(0, 0, 0, 0);
	sampleMask = 0xFFFFFFFFu;

	_graphType = GT_BLEND_STATE;

	for(int i = 0; i < NUM_TARGETS; ++i){
		//获取引用, 否则就是拷贝
		Target& trg = target[i];
		trg.enable = false;
		trg.srcColor = BM_ONE;
		trg.dstColor = BM_ZERO;
		trg.opColor = OP_ADD;
		trg.srcAlpha = BM_ONE;
		trg.dstAlpha = BM_ZERO;
		trg.opAlpha = OP_ADD;
		trg.mask = CW_ENABLE_ALL;
	}
}


BlendState::~BlendState(){
}
