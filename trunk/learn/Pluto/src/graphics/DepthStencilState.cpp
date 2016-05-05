#include <graphics/DepthStencilState.h>
using namespace plu;

DepthStencilState::DepthStencilState(){
	depthEnable = true;
	writeMask = MASK_ALL;
	comparison = LESS_EQUAL;
	
	stencilEnable = false;
	stencilReadMask = 0xFF;
	stencilWriteMask = 0xFF;
	reference = 0;

	_graphType = GT_DEPTH_STENCIL_STATE;

	frontFace.fail = OP_KEEP;
	frontFace.pass = OP_KEEP;
	frontFace.depthFail = OP_KEEP;
	frontFace.comparison = ALWAYS;
	backFace = frontFace;
}


DepthStencilState::~DepthStencilState()
{
}
