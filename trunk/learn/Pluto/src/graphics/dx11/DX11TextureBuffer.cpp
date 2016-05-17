#include <graphics/dx11/DX11TextureBuffer.h>
using namespace plu;


DX11TextureBuffer::~DX11TextureBuffer(){
	if(mSRView){
		mSRView->Release();
		mSRView = nullptr;
	}
}

DX11TextureBuffer::DX11TextureBuffer(ID3D11Device* device, TextureBuffer* tbuffer)
    :DX11Buffer(tbuffer){
	mSRView = nullptr;

    // Specify the buffer description.
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = tbuffer->GetTotalBytes();
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    GraphicsResource::Usage usage = tbuffer->GetUsage();
    if (usage == GraphicsResource::IMMUTABLE){
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.CPUAccessFlags = 0;
    }else if (usage == GraphicsResource::DYNAMIC_UPDATE){
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }else{
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
    }

    // Create the buffer.
    ID3D11Buffer* buffer = nullptr;
    HRESULT hr;
    if (tbuffer->GetData()){
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = tbuffer->GetData();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        hr = device->CreateBuffer(&desc, &data, &buffer);
    }else{
        hr = device->CreateBuffer(&desc, nullptr, &buffer);
    }
    _dxObj = buffer;

    CreateSRView(device);

    // Create a staging buffer if requested.
    if (tbuffer->GetCopyType() != GraphicsResource::COPY_NONE){
        CreateStaging(device, desc);
    }
}

DX11TextureBuffer* DX11TextureBuffer::Create(ID3D11Device* device, GraphicsObject* object){
    if (object->GetType() == GT_TEXTURE_BUFFER){
        return new DX11TextureBuffer(device, static_cast<TextureBuffer*>(object));
    }

    return nullptr;
}

TextureBuffer* DX11TextureBuffer::GetTextureBuffer(){
    return static_cast<TextureBuffer*>(_pluObj);
}

ID3D11ShaderResourceView* DX11TextureBuffer::GetSRView(){
    return mSRView;
}

void DX11TextureBuffer::CreateSRView(ID3D11Device* device){
    TextureBuffer* tbuffer = GetTextureBuffer();
    ID3D11Buffer* dxBuffer = GetDXBuffer();

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    desc.Format = static_cast<DXGI_FORMAT>(tbuffer->GetFormat());
    desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    desc.Buffer.FirstElement = 0;
    desc.Buffer.NumElements = tbuffer->GetItemNum();
    HRESULT hr = device->CreateShaderResourceView(dxBuffer, &desc, &mSRView);
    //CHECK_HR_RETURN_NONE("Failed to create shader resource view");
}

void DX11TextureBuffer::SetName(std::string const& name){
    DX11Buffer::SetName(name);
    //HRESULT hr = SetPrivateName(mSRView, name);
}

