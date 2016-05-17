#include <graphics/dx11/DX11ConstantBuffer.h>
using namespace plu;

DX11ConstantBuffer::~DX11ConstantBuffer(){
}

DX11ConstantBuffer::DX11ConstantBuffer(ID3D11Device* device,
    ConstantBuffer* cbuffer)
    :DX11Buffer(cbuffer){
    // Specify the buffer description.
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = cbuffer->GetTotalBytes();
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    GraphicsResource::Usage usage = cbuffer->GetUsage();
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
    if (cbuffer->GetData()){
		//设置resource data, 用cbuffer的数据建立d3d 缓存
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = cbuffer->GetData();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        hr = device->CreateBuffer(&desc, &data, &buffer);
    }else{
        hr = device->CreateBuffer(&desc, nullptr, &buffer);
    }
    _dxObj = buffer;

    // Create a staging buffer if requested.
	//TODO, 如果buff为不拷贝, 为何要创建stateing buff, create stating创建了一个支持从gpu拷贝的cpu的buff
    if (cbuffer->GetCopyType() != GraphicsResource::COPY_NONE){
        CreateStaging(device, desc);
    }
}

DX11ConstantBuffer* DX11ConstantBuffer::Create(ID3D11Device* device, GraphicsObject* object){
    if (object->GetType() == GT_CONSTANT_BUFFER)
        return new DX11ConstantBuffer(device, static_cast<ConstantBuffer*>(object));

    //LogError("Invalid object type.");
    return nullptr;
}

