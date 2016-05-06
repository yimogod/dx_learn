#include <graphics/RasterizerState.h>
using namespace plu;

RasterizerState::RasterizerState(){
	fillMode = FILL_SOLID;
	cullMode = CULL_BACK;
	frontCCW = true;
	depthBias = 0;
	depthBiasClamp = 0.0f;
	slopeScaledDepthBias = 0.0f;
	enableDepthClip = true;
	enableScissor = false;
	enableMultisample = false;
	enableAntialiasedLine = false;
    _graphType = GT_RASTERIZER_STATE;
}
