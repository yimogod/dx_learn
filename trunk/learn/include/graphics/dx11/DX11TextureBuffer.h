#pragma once
#include <graphics/TextureBuffer.h>
#include <graphics/dx11/DX11Buffer.h>

namespace plu
{

class DX11TextureBuffer : public DX11Buffer
{
public:
    // Construction and destruction.
    virtual ~DX11TextureBuffer();
    DX11TextureBuffer(ID3D11Device* device, TextureBuffer* tbuffer);

    static DX11TextureBuffer* Create(ID3D11Device* device, GraphicsObject* object);

    // Member access.
    TextureBuffer* GetTextureBuffer();
    ID3D11ShaderResourceView* GetSRView();

private:
    void CreateSRView(ID3D11Device* device);

    ID3D11ShaderResourceView* mSRView;

public:
    // Support for the DX11 debug layer; see comments in the file
    // GteDX11GraphicsObject.h about usage.
    virtual void SetName(std::string const& name);
};

}
