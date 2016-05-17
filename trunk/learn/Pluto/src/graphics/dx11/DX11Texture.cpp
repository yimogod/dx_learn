#include <graphics/dx11/DX11Texture.h>
using namespace plu;


DX11Texture::~DX11Texture(){
	if(mSRView){
		mSRView->Release();
		mSRView = nullptr;
	}

	if(mUAView){
		mUAView->Release();
		mUAView = nullptr;
	}
}

DX11Texture::DX11Texture(Texture* gtTexture)
    :DX11Resource(gtTexture){
	mSRView = nullptr;
	mUAView = nullptr;
}

Texture* DX11Texture::GetTexture(){
    return static_cast<Texture*>(_pluObj);
}

ID3D11ShaderResourceView* DX11Texture::GetSRView(){
    return mSRView;
}

ID3D11UnorderedAccessView* DX11Texture::GetUAView(){
    return mUAView;
}

bool DX11Texture::Update(ID3D11DeviceContext* context, int sri)
{
    Texture* texture = GetTexture();
    if (sri >= texture->GetNumSubresources()){
        //gWarning("Subresource index out of range.");
        return false;
    }

    if (texture->GetUsage() != GraphicsResource::DYNAMIC_UPDATE){
        //LogWarning("Texture usage is not DYNAMIC_UPDATE.");
        return false;
    }

    // Map the texture.
    ID3D11Resource* dxTexture = GetDXResource();
    D3D11_MAPPED_SUBRESOURCE sub;
    HRESULT hr = context->Map(dxTexture, sri, D3D11_MAP_WRITE_DISCARD, 0, &sub);
    //CHECK_HR_RETURN("Failed to map staging texture", false);

    // Copy from CPU memory.
    auto sr = texture->GetSubresource(sri);
    unsigned int numDimensions = texture->GetNumDimensions();
    if (numDimensions == 1)
    {
        //Memcpy(sub.pData, sr.data, texture->GetNumBytesFor(sr.level));
    }
    else if (numDimensions == 2)
    {
        CopyPitched2(texture->GetDimensionFor(sr.level, 1), sr.rowPitch,
            sr.data, sub.RowPitch, sub.pData);
    }
    else  // numDimensions == 3
    {
        CopyPitched3(texture->GetDimensionFor(sr.level, 1),
            texture->GetDimensionFor(sr.level, 2), sr.rowPitch, sr.slicePitch,
            sr.data, sub.RowPitch, sub.DepthPitch, sub.pData);
    }
    context->Unmap(dxTexture, sri);
    return true;
}

bool DX11Texture::Update(ID3D11DeviceContext* context){
    Texture* texture = GetTexture();
    unsigned int const numSubresources = texture->GetNumSubresources();
    for (unsigned int index = 0; index < numSubresources; ++index)
    {
        if (!Update(context, index))
        {
            return false;
        }
    }
    return true;
}

bool DX11Texture::CopyCpuToGpu(ID3D11DeviceContext* context, int sri){
    Texture* texture = GetTexture();
    if (sri >= texture->GetNumSubresources())
    {
        //LogWarning("Subresource index out of range.");
        return false;
    }

    if (!PreparedForCopy(D3D11_CPU_ACCESS_WRITE))
    {
        return false;
    }

    // Map the staging texture.
    D3D11_MAPPED_SUBRESOURCE sub;
    HRESULT hr = context->Map(mStaging, sri, D3D11_MAP_WRITE, 0, &sub);
    //CHECK_HR_RETURN("Failed to map staging texture", false);

    // Copy from CPU memory to staging texture.
    auto sr = texture->GetSubresource(sri);
    unsigned int numDimensions = texture->GetNumDimensions();
    if (numDimensions == 1){
        //Memcpy(sub.pData, sr.data, texture->GetNumBytesFor(sr.level));
    }else if (numDimensions == 2){
        CopyPitched2(texture->GetDimensionFor(sr.level, 1), sr.rowPitch,
            sr.data, sub.RowPitch, sub.pData);
    }else{  // numDimensions == 3
        CopyPitched3(texture->GetDimensionFor(sr.level, 1),
            texture->GetDimensionFor(sr.level, 2), sr.rowPitch,
            sr.slicePitch, sr.data, sub.RowPitch, sub.DepthPitch,
            sub.pData);
    }
    context->Unmap(mStaging, sri);

    // Copy from staging texture to GPU memory.
    ID3D11Resource* dxTexture = GetDXResource();
    context->CopySubresourceRegion(dxTexture, sri, 0, 0, 0, mStaging, sri,
        nullptr);
    return true;
}

bool DX11Texture::CopyCpuToGpu(ID3D11DeviceContext* context){
    Texture* texture = GetTexture();
    int const numSubresources = texture->GetNumSubresources();
    for (int index = 0; index < numSubresources; ++index){
        if (!CopyCpuToGpu(context, index))
        {
            return false;
        }
    }

    // Generate mipmaps (when they exist).
    if (texture->WantAutogenerateMipmaps() && mSRView){
        context->GenerateMips(mSRView);
    }
    return true;
}

bool DX11Texture::CopyGpuToCpu(ID3D11DeviceContext* context, int sri){
    Texture* texture = GetTexture();
    if (sri >= texture->GetNumSubresources()){
        //LogWarning("Subresource index out of range.");
        return false;
    }

    if (!PreparedForCopy(D3D11_CPU_ACCESS_READ)){
        return false;
    }

    // Copy from GPU memory to staging texture.
    ID3D11Resource* dxTexture = GetDXResource();
    context->CopySubresourceRegion(mStaging, sri, 0, 0, 0, dxTexture, sri,
        nullptr);

    // Map the staging texture.
    D3D11_MAPPED_SUBRESOURCE sub;
    HRESULT hr = context->Map(mStaging, sri, D3D11_MAP_READ, 0, &sub);
    //CHECK_HR_RETURN("Failed to map staging texture", false);

    // Copy from staging texture to CPU memory.
    auto sr = texture->GetSubresource(sri);
    unsigned int numDimensions = texture->GetNumDimensions();
    if (numDimensions == 1)
    {
        //Memcpy(sr.data, sub.pData, texture->GetNumBytesFor(sr.level));
    }else if (numDimensions == 2){
        CopyPitched2(texture->GetDimensionFor(sr.level, 1), sub.RowPitch,
            sub.pData, sr.rowPitch, sr.data);
    }else{  // numDimensions == 3
        CopyPitched3(texture->GetDimensionFor(sr.level, 1),
            texture->GetDimensionFor(sr.level, 2), sub.RowPitch,
            sub.DepthPitch, sub.pData, sr.rowPitch, sr.slicePitch, sr.data);
    }
    context->Unmap(mStaging, sri);
    return true;
}

bool DX11Texture::CopyGpuToCpu(ID3D11DeviceContext* context){
    Texture* texture = GetTexture();
    int const numSubresources = texture->GetNumSubresources();
    for (int index = 0; index < numSubresources; ++index){
        if (!CopyGpuToCpu(context, index))
        {
            return false;
        }
    }
    return true;
}

void DX11Texture::CopyPitched2(int numRows, int srcRowPitch, void* srcData, int trgRowPitch, void* trgData){
    if (srcRowPitch == trgRowPitch){
        // The memory is contiguous.
        //Memcpy(trgData, srcData, trgRowPitch*numRows);
    }else{
        // Padding was added to each row of the texture, so we must
        // copy a row at a time to compensate for differing pitches.
        int numRowBytes = std::min(srcRowPitch, trgRowPitch);
        char* srcRow = static_cast<char*>(srcData);
        char* trgRow = static_cast<char*>(trgData);
        for (int row = 0; row < numRows; ++row){
            //Memcpy(trgRow, srcRow, numRowBytes);
            srcRow += srcRowPitch;
            trgRow += trgRowPitch;
        }
    }
}

void DX11Texture::CopyPitched3(int numRows,
    int numSlices, int srcRowPitch,
    int srcSlicePitch, void* srcData, int trgRowPitch,
    int trgSlicePitch, void* trgData){
    if (srcRowPitch == trgRowPitch && srcSlicePitch == trgSlicePitch)
    {
        // The memory is contiguous.
        //Memcpy(trgData, srcData, trgSlicePitch*numSlices);
    }
    else
    {
        // Padding was added to each row and/or slice of the texture, so
        // we must copy the data to compensate for differing pitches.
        unsigned int numRowBytes = std::min(srcRowPitch, trgRowPitch);
        char* srcSlice = static_cast<char*>(srcData);
        char* trgSlice = static_cast<char*>(trgData);
        for (unsigned int slice = 0; slice < numSlices; ++slice){
            char* srcRow = srcSlice;
            char* trgRow = trgSlice;
            for (int row = 0; row < numRows; ++row){
                //Memcpy(trgRow, srcRow, numRowBytes);
                srcRow += srcRowPitch;
                trgRow += trgRowPitch;
            }
            srcSlice += srcSlicePitch;
            trgSlice += trgSlicePitch;
        }
    }
}

void DX11Texture::SetName(std::string const& name){
    //DX11Resource::SetName(name);
    //HRESULT hr = SetPrivateName(mSRView, name);
    //CHECK_HR_RETURN_NONE("Failed to set private name");
    //hr = SetPrivateName(mUAView, name);
    //CHECK_HR_RETURN_NONE("Failed to set private name");
}

