#include <graphics/dx11/DX11IndexBuffer.h>
using namespace plu;

DX11IndexBuffer::~DX11IndexBuffer(){
}

DX11IndexBuffer::DX11IndexBuffer(ID3D11Device* device, IndexBuffer* ibuffer)
    :DX11Buffer(ibuffer){
	mFormat = ibuffer->GetItemSize() == sizeof(int) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
    
	// Specify the buffer description.
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = ibuffer->GetTotalBytes();
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    GraphicsResource::Usage usage = ibuffer->GetUsage();
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
    if (ibuffer->GetData()){
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = ibuffer->GetData();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        hr = device->CreateBuffer(&desc, &data, &buffer);
    }else{
        hr = device->CreateBuffer(&desc, nullptr, &buffer);
    }

    _dxObj = buffer;
    // Create a staging buffer if requested.
    if (ibuffer->GetCopyType() != GraphicsResource::COPY_NONE){
        CreateStaging(device, desc);
    }
}

DX11IndexBuffer* DX11IndexBuffer::Create(ID3D11Device* device, GraphicsObject* object){
    if (object->GetType() == GT_INDEX_BUFFER){
        return new DX11IndexBuffer(device, static_cast<IndexBuffer*>(object));
    }
    return nullptr;
}

IndexBuffer* DX11IndexBuffer::GetIndexBuffer() const{
    return static_cast<IndexBuffer*>(_pluObj);
}

void DX11IndexBuffer::Enable(ID3D11DeviceContext* context){
    if (_dxObj){
        ID3D11Buffer* dxBuffer = static_cast<ID3D11Buffer*>(_dxObj);
        context->IASetIndexBuffer(dxBuffer, mFormat, 0);
    }
}

void DX11IndexBuffer::Disable(ID3D11DeviceContext* context){
    if (_dxObj){
        context->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
    }
}

