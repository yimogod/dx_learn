#include <graphics/dx11/DX11SamplerState.h>
using namespace plu;

DX11SamplerState::~DX11SamplerState(){
}

DX11SamplerState::DX11SamplerState(ID3D11Device* device, SamplerState* samplerState)
    :DX11DrawingState(samplerState){
    // Specify the sampler state description.
    D3D11_SAMPLER_DESC desc;
    desc.Filter = msFilter[samplerState->filter];
    desc.AddressU = msMode[samplerState->mode[0]];
    desc.AddressV = msMode[samplerState->mode[1]];
    desc.AddressW = msMode[samplerState->mode[2]];
    desc.MipLODBias = samplerState->mipLODBias;
    desc.MaxAnisotropy = samplerState->maxAnisotropy;
    desc.ComparisonFunc = msComparison[samplerState->comparison];
    //desc.BorderColor[0] = samplerState->borderColor[0];
    //desc.BorderColor[1] = samplerState->borderColor[1];
    //desc.BorderColor[2] = samplerState->borderColor[2];
    //desc.BorderColor[3] = samplerState->borderColor[3];
    desc.MinLOD = samplerState->minLOD;
    desc.MaxLOD = samplerState->maxLOD;

    // Create the sampler state.
    ID3D11SamplerState* state = nullptr;
    device->CreateSamplerState(&desc, &state);
    _dxObj = state;
}

DX11SamplerState* DX11SamplerState::create(ID3D11Device* device, GraphicsObject* object){
    if (object->getType() != GT_SAMPLER_STATE){
		return nullptr;
    }
    return new DX11SamplerState(device, static_cast<SamplerState*>(object));

}

SamplerState* DX11SamplerState::getSamplerState(){
    return static_cast<SamplerState*>(_pluObj);
}

ID3D11SamplerState* DX11SamplerState::getDXSamplerState(){
    return static_cast<ID3D11SamplerState*>(_dxObj);
}


D3D11_FILTER const DX11SamplerState::msFilter[] = {
    D3D11_FILTER_MIN_MAG_MIP_POINT,
    D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
    D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
    D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
    D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
    D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
    D3D11_FILTER_MIN_MAG_MIP_LINEAR,
    D3D11_FILTER_ANISOTROPIC,
    D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT,
    D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
    D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
    D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
    D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
    D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
    D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
    D3D11_FILTER_COMPARISON_ANISOTROPIC
};

D3D11_TEXTURE_ADDRESS_MODE const DX11SamplerState::msMode[] = {
    D3D11_TEXTURE_ADDRESS_WRAP,
    D3D11_TEXTURE_ADDRESS_MIRROR,
    D3D11_TEXTURE_ADDRESS_CLAMP,
    D3D11_TEXTURE_ADDRESS_BORDER,
    D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
};

D3D11_COMPARISON_FUNC const DX11SamplerState::msComparison[] = {
    D3D11_COMPARISON_NEVER,
    D3D11_COMPARISON_LESS,
    D3D11_COMPARISON_EQUAL,
    D3D11_COMPARISON_LESS_EQUAL,
    D3D11_COMPARISON_GREATER,
    D3D11_COMPARISON_NOT_EQUAL,
    D3D11_COMPARISON_GREATER_EQUAL,
    D3D11_COMPARISON_ALWAYS
};
