#include <graphics/dx11/DX11RasterizerState.h>
using namespace plu;

DX11RasterizerState::~DX11RasterizerState(){
}

DX11RasterizerState::DX11RasterizerState(ID3D11Device* device, RasterizerState* rasterizerState)
    :DX11DrawingState(rasterizerState){
    // Specify the rasterizer state description.
    D3D11_RASTERIZER_DESC desc;
    desc.FillMode = msFillMode[rasterizerState->fillMode];
    desc.CullMode = msCullMode[rasterizerState->cullMode];
    desc.FrontCounterClockwise = (rasterizerState->frontCCW ? TRUE : FALSE);
    desc.DepthBias = rasterizerState->depthBias;
    desc.DepthBiasClamp = rasterizerState->depthBiasClamp;
    desc.SlopeScaledDepthBias = rasterizerState->slopeScaledDepthBias;
    desc.DepthClipEnable = (rasterizerState->enableDepthClip ? TRUE : FALSE);
    desc.ScissorEnable = (rasterizerState->enableScissor ? TRUE : FALSE);
    desc.MultisampleEnable = 
        (rasterizerState->enableMultisample ? TRUE : FALSE);
    desc.AntialiasedLineEnable =
        (rasterizerState->enableAntialiasedLine ? TRUE : FALSE);

    // Create the rasterizer state.
    ID3D11RasterizerState* state = nullptr;
    device->CreateRasterizerState(&desc, &state);
    _dxObj = state;
}

DX11RasterizerState* DX11RasterizerState::create(ID3D11Device* device, GraphicsObject* object){
    if (object->getType() != GT_RASTERIZER_STATE){
		return nullptr;
    }

    return new DX11RasterizerState(device, static_cast<RasterizerState*>(object));

}

RasterizerState* DX11RasterizerState::getRasterizerState(){
    return static_cast<RasterizerState*>(_pluObj);
}

ID3D11RasterizerState* DX11RasterizerState::getDXRasterizerState(){
    return static_cast<ID3D11RasterizerState*>(_dxObj);
}

void DX11RasterizerState::enable(ID3D11DeviceContext* context){
    context->RSSetState(getDXRasterizerState());
}


D3D11_FILL_MODE const DX11RasterizerState::msFillMode[] = {
    D3D11_FILL_SOLID,
    D3D11_FILL_WIREFRAME
};

D3D11_CULL_MODE const DX11RasterizerState::msCullMode[] = {
    D3D11_CULL_NONE,
    D3D11_CULL_FRONT,
    D3D11_CULL_BACK
};
