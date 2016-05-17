#include <graphics/dx11/DX11VertexBuffer.h>
using namespace plu;

DX11VertexBuffer::~DX11VertexBuffer(){
}

DX11VertexBuffer::DX11VertexBuffer(ID3D11Device* device, VertexBuffer* vbuffer)
    :DX11Buffer(vbuffer){

    // Specify the buffer description.
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = vbuffer->GetTotalBytes();
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    GraphicsResource::Usage usage = vbuffer->GetUsage();
    if (usage == GraphicsResource::IMMUTABLE){
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.CPUAccessFlags = 0;
    }else if (usage == GraphicsResource::DYNAMIC_UPDATE){
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }else{
		// usage == Resource::SHADER_OUTPUT
        // TODO: Write a sample application to test this case.
        //LogError("Vertex output streams are not yet tested.");
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
        desc.CPUAccessFlags = 0;
    }

    // Create the buffer.
    ID3D11Buffer* buffer = nullptr;
    HRESULT hr;
    if (vbuffer->GetData()){
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = vbuffer->GetData();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        hr = device->CreateBuffer(&desc, &data, &buffer);
    }else{
        hr = device->CreateBuffer(&desc, nullptr, & buffer);
    }
    //CHECK_HR_RETURN_NONE("Failed to create vertex buffer");
    _dxObj = buffer;

    // Create a staging buffer if requested.
    if (vbuffer->GetCopyType() != GraphicsResource::COPY_NONE){
        CreateStaging(device, desc);
    }
}

DX11VertexBuffer* DX11VertexBuffer::Create(ID3D11Device* device, GraphicsObject* object){
    if (object->GetType() == GT_VERTEX_BUFFER){
        return new DX11VertexBuffer(device, static_cast<VertexBuffer*>(object));
    }

    return nullptr;
}

VertexBuffer* DX11VertexBuffer::GetVertexBuffer(){
    return static_cast<VertexBuffer*>(_pluObj);
}

void DX11VertexBuffer::Enable(ID3D11DeviceContext* context){
    if (_dxObj){
        // The MSDN documentation for ID3D11DeviceContext::IASetVertexBuffers
        // and ID3D11DeviceContext::Draw(numVertices, startVertex) appears
        // not to mention that startVertex is relative to the offsets[]
        // passed to IASetVertexBuffers.  If you set the offsets[0] here, you
        // should call Draw(numVertices,0).  If you instead call
        // Draw(numVertices, startVertex), then you should set offsets[0]
        // to 0.  The latter choice is made for GTEngine.  TODO:  Is there a
        // performance issue by setting offsets[0] to zero?  This depends on
        // what the input assembly stage does with the buffers when you
        // enable them using IASetVertexBuffers.
        ID3D11Buffer* buffers[1] = { GetDXBuffer() };
        VertexBuffer* vbuffer = GetVertexBuffer();
        UINT strides[1] = { vbuffer->GetItemSize() };
        UINT offsets[1] = { 0 };
        context->IASetVertexBuffers(0, 1, buffers, strides, offsets);
    }
}

void DX11VertexBuffer::Disable(ID3D11DeviceContext* context){
    if (_dxObj){
        ID3D11Buffer* buffers[1] = { nullptr };
        UINT strides[1] = { 0 };
        UINT offsets[1] = { 0 };
        context->IASetVertexBuffers(0, 1, buffers, strides, offsets);
    }
}