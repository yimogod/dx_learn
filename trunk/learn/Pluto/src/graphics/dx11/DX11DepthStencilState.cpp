#include <graphics/dx11/DX11DepthStencilState.h>
using namespace plu;

DX11DepthStencilState::~DX11DepthStencilState(){
}

DX11DepthStencilState::DX11DepthStencilState(ID3D11Device* device, DepthStencilState* state)
	:DX11DrawingState(state){
	// Specify the rasterizer state description.
	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = (state->depthEnable ? TRUE : FALSE);
	desc.DepthWriteMask = msWriteMask[state->writeMask];
	desc.DepthFunc = msComparison[state->comparison];
	desc.StencilEnable = (state->stencilEnable ? TRUE : FALSE);
	desc.StencilReadMask = state->stencilReadMask;
	desc.StencilWriteMask = state->stencilWriteMask;

	DepthStencilState::Face front = state->frontFace;
	desc.FrontFace.StencilFailOp = msOperation[front.fail];
	desc.FrontFace.StencilDepthFailOp = msOperation[front.depthFail];
	desc.FrontFace.StencilPassOp = msOperation[front.pass];
	desc.FrontFace.StencilFunc = msComparison[front.comparison];

	DepthStencilState::Face back = state->backFace;
	desc.BackFace.StencilFailOp = msOperation[back.fail];
	desc.BackFace.StencilDepthFailOp = msOperation[back.depthFail];
	desc.BackFace.StencilPassOp = msOperation[back.pass];
	desc.BackFace.StencilFunc = msComparison[back.comparison];

	// Create the depth-stencil state.
	ID3D11DepthStencilState* ds = nullptr;
	device->CreateDepthStencilState(&desc, &ds);
	_dxObj = ds;
}

DX11DepthStencilState* DX11DepthStencilState::create(ID3D11Device* device, GraphicsObject* object){
	if(object->getType() == GT_DEPTH_STENCIL_STATE){
		return nullptr;
	}
	return new DX11DepthStencilState(device, static_cast<DepthStencilState*>(object));

}

DepthStencilState* DX11DepthStencilState::getDepthStencilState()
{
	return static_cast<DepthStencilState*>(_pluObj);
}

ID3D11DepthStencilState* DX11DepthStencilState::getDXDepthStencilState()
{
	return static_cast<ID3D11DepthStencilState*>(_dxObj);
}

void DX11DepthStencilState::enable(ID3D11DeviceContext* context)
{
	DepthStencilState* dsState = getDepthStencilState();
	context->OMSetDepthStencilState(getDXDepthStencilState(), dsState->reference);
}


D3D11_DEPTH_WRITE_MASK const DX11DepthStencilState::msWriteMask[] =
{
	D3D11_DEPTH_WRITE_MASK_ZERO,
	D3D11_DEPTH_WRITE_MASK_ALL
};

D3D11_COMPARISON_FUNC const DX11DepthStencilState::msComparison[] =
{
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_EQUAL,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_NOT_EQUAL,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_ALWAYS
};

D3D11_STENCIL_OP const DX11DepthStencilState::msOperation[] =
{
	D3D11_STENCIL_OP_KEEP,
	D3D11_STENCIL_OP_ZERO,
	D3D11_STENCIL_OP_REPLACE,
	D3D11_STENCIL_OP_INCR_SAT,
	D3D11_STENCIL_OP_DECR_SAT,
	D3D11_STENCIL_OP_INVERT,
	D3D11_STENCIL_OP_INCR,
	D3D11_STENCIL_OP_DECR
};
