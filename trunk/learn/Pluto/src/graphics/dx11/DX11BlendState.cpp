#include <graphics/dx11/DX11BlendState.h>
using namespace plu;

DX11BlendState::DX11BlendState(ID3D11Device* device, BlendState* state):
	DX11DrawingState(state){
	// Specify the blend state description.
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	desc.AlphaToCoverageEnable = (state->enableAlphaToCoverage ? TRUE : FALSE);
	desc.IndependentBlendEnable = (state->enableIndependentBlend ? TRUE : FALSE);

	for(unsigned int i = 0; i < BlendState::NUM_TARGETS; ++i){
		D3D11_RENDER_TARGET_BLEND_DESC& out = desc.RenderTarget[i];
		BlendState::Target const& in = state->target[i];
		out.BlendEnable = (in.enable ? TRUE : FALSE);
		out.SrcBlend = msMode[in.srcColor];
		out.DestBlend = msMode[in.dstColor];
		out.BlendOp = msOperation[in.opColor];
		out.SrcBlendAlpha = msMode[in.srcAlpha];
		out.DestBlendAlpha = msMode[in.dstAlpha];
		out.BlendOpAlpha = msOperation[in.opAlpha];

		out.RenderTargetWriteMask = in.mask;
	}

	// Create the blend state.
	ID3D11BlendState* bs = nullptr;
	device->CreateBlendState(&desc, &bs);
	_dxObj = bs;
}

DX11BlendState::~DX11BlendState(){
}

DX11BlendState* DX11BlendState::create(ID3D11Device* device, GraphicsObject* obj){
	if(obj->getType() != GT_BLEND_STATE){
		return nullptr;
	}

	return new DX11BlendState(device, static_cast<BlendState*>(obj));
}

BlendState* DX11BlendState::getBlendState(){
	return static_cast<BlendState*>(_pluObj);
}

ID3D11BlendState* DX11BlendState::getDXBlendState(){
	return static_cast<ID3D11BlendState*>(_dxObj);
}

void DX11BlendState::enable(ID3D11DeviceContext* context){
	BlendState* bState = getBlendState();
	float blendFactor[4] = { 0, 0, 0, 0 };
	context->OMSetBlendState(getDXBlendState(), blendFactor, bState->sampleMask);
}

D3D11_BLEND const DX11BlendState::msMode[] = {
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR,
	D3D11_BLEND_SRC_ALPHA_SAT,
	D3D11_BLEND_BLEND_FACTOR,
	D3D11_BLEND_INV_BLEND_FACTOR,
	D3D11_BLEND_SRC1_COLOR,
	D3D11_BLEND_INV_SRC1_COLOR,
	D3D11_BLEND_SRC1_ALPHA,
	D3D11_BLEND_INV_SRC1_ALPHA
};

D3D11_BLEND_OP const DX11BlendState::msOperation[] = {
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX
};
