#include <graphics/SamplerState.h>
using namespace plu;

SamplerState::SamplerState(){
	filter = MIN_P_MAG_P_MIP_P;
	mipLODBias = 0.0f;
	maxAnisotropy = 1;
	comparison = NEVER;
	minLOD = 0;
	maxLOD = 10000000;

    _graphType = GT_SAMPLER_STATE;

    mode[0] = CLAMP;
    mode[1] = CLAMP;
    mode[2] = CLAMP;
}

