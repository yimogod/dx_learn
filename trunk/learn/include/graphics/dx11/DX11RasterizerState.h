#pragma once

#include <graphics/RasterizerState.h>
#include <graphics/dx11/DX11DrawingState.h>

namespace plu{

class DX11RasterizerState : public DX11DrawingState{
public:
    // Construction and destruction.
    virtual ~DX11RasterizerState();
    DX11RasterizerState(ID3D11Device* device, RasterizerState* rasterizerState);
    static DX11RasterizerState* create(ID3D11Device* device, GraphicsObject* object);

    // Member access.
    RasterizerState* getRasterizerState();
    ID3D11RasterizerState* getDXRasterizerState();

    // Enable the rasterizer state.
    void enable(ID3D11DeviceContext* context);

private:
    // Conversions from GTEngine values to DX11 values.
    static D3D11_FILL_MODE const msFillMode[];
    static D3D11_CULL_MODE const msCullMode[];
};

}
