#include <graphics/dx11/DX11Buffer.h>
#include <cstring>
using namespace plu;

DX11Buffer::~DX11Buffer(){
}

DX11Buffer::DX11Buffer(Buffer* buffer)
    :DX11Resource(buffer){
}

Buffer* DX11Buffer::GetBuffer(){
    return static_cast<Buffer*>(_pluObj);
}

ID3D11Buffer* DX11Buffer::GetDXBuffer(){
    return static_cast<ID3D11Buffer*>(_dxObj);
}

bool DX11Buffer::Update(ID3D11DeviceContext* context){
    Buffer* buffer = GetBuffer();
    if (buffer->GetUsage() != GraphicsResource::DYNAMIC_UPDATE){
        //LogWarning("Buffer usage is not DYNAMIC_UPDATE.");
        return false;
    }

	int numActiveBytes = buffer->GetTotalBytes();
    if (numActiveBytes > 0){
        // Map the buffer.
        ID3D11Buffer* dxBuffer = GetDXBuffer();
        D3D11_MAPPED_SUBRESOURCE sub;
        HRESULT hr = context->Map(dxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
        //CHECK_HR_RETURN("Failed to map buffer", false);

        // Copy from CPU memory.
        //int offset = buffer->GetOffset();
		int offset = 0;
        //Memcpy(sub.pData, buffer->GetData() + offset, numActiveBytes);
        context->Unmap(dxBuffer, 0);
		return true;
    }

	//LogInformation("Buffer has zero active bytes.");
	return false;
}

bool DX11Buffer::CopyCpuToGpu(ID3D11DeviceContext* context){
    if (!PreparedForCopy(D3D11_CPU_ACCESS_WRITE)){
        return false;
    }

    Buffer* buffer = GetBuffer();
    int numActiveBytes = buffer->GetTotalBytes();
    if (numActiveBytes > 0){
        // Map the staging buffer.
        D3D11_MAPPED_SUBRESOURCE sub;
        HRESULT hr = context->Map(mStaging, 0, D3D11_MAP_WRITE, 0, &sub);
        //CHECK_HR_RETURN("Failed to map staging buffer", false);

        // Copy from CPU memory to staging buffer.
        //Memcpy(sub.pData, buffer->GetData(), numActiveBytes);
        context->Unmap(mStaging, 0);

        // Copy from staging buffer to GPU memory.
        //int offset = buffer->GetOffset();
		int offset = 0;
        D3D11_BOX box = { offset, 0, 0, numActiveBytes, 1, 1 };
        context->CopySubresourceRegion(GetDXBuffer(), 0, offset, 0, 0,
            mStaging, 0, &box);
		return true;
    }
    //LogInformation("Buffer has zero active bytes.");
	return false;
}

bool DX11Buffer::CopyGpuToCpu(ID3D11DeviceContext* context){
    if (!PreparedForCopy(D3D11_CPU_ACCESS_READ)){
        return false;
    }

    Buffer* buffer = GetBuffer();
    UINT numActiveBytes = buffer->GetTotalBytes();
    if (numActiveBytes > 0){
        // Copy from GPU memory to staging buffer.
        //unsigned int offset = buffer->GetOffset();
		int offset = 0;
        D3D11_BOX box = { offset, 0, 0, numActiveBytes, 1, 1 };
        context->CopySubresourceRegion(mStaging, 0, offset, 0, 0,
            GetDXBuffer(), 0, &box);

        // Map the staging buffer.
        D3D11_MAPPED_SUBRESOURCE sub;
        HRESULT hr = context->Map(mStaging, 0, D3D11_MAP_READ, 0, &sub);
        //CHECK_HR_RETURN("Failed to map staging buffer", false);

        // Copy from staging buffer to CPU memory.
        //Memcpy(buffer->GetData(), sub.pData, numActiveBytes);
        context->Unmap(mStaging, 0);
		return true;
    }
    //LogInformation("Buffer has zero active bytes.");
	return false;
}

bool DX11Buffer::Update(ID3D11DeviceContext*, int){
    //LogError("This was called polymorphically through DX11Resource.");
    return false;
}

bool DX11Buffer::CopyCpuToGpu(ID3D11DeviceContext*, int){
    //LogError("This was called polymorphically through DX11Resource.");
    return false;
}

bool DX11Buffer::CopyGpuToCpu(ID3D11DeviceContext*, int){
    //LogError("This was called polymorphically through DX11Resource.");
    return false;
}

void DX11Buffer::CreateStaging(ID3D11Device* device, D3D11_BUFFER_DESC& bf){
    D3D11_BUFFER_DESC desc;

	//这个缓存支持数据从gpu拷贝到cpu
    desc.ByteWidth = bf.ByteWidth;
    desc.Usage = D3D11_USAGE_STAGING;//gpu->cpu
    desc.BindFlags = 0;
    desc.CPUAccessFlags = msStagingAccess[GetBuffer()->GetCopyType()];
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    HRESULT hr = device->CreateBuffer(&desc, nullptr,
        reinterpret_cast<ID3D11Buffer**>(&mStaging));
    //CHECK_HR_RETURN_NONE("Failed to create staging buffer");
}

