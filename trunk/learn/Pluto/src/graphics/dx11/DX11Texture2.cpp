#include <graphics/DX11/DX11Texture2.h>
using namespace plu;

DX11Texture2::~DX11Texture2(){
}

DX11Texture2::DX11Texture2(ID3D11Device* device, Texture2* texture)
    :DX11TextureSingle(texture){
    // Specify the texture description.
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = texture->GetWidth();
    desc.Height = texture->GetHeight();
    desc.MipLevels = texture->GetNumLevels();
    desc.ArraySize = 1;
    desc.Format = static_cast<DXGI_FORMAT>(texture->GetFormat());
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.MiscFlags = (texture->IsShared() ? D3D11_RESOURCE_MISC_SHARED : 0);
    
	GraphicsResource::Usage usage = texture->GetUsage();
    if (usage == GraphicsResource::IMMUTABLE){
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.CPUAccessFlags = 0;
    }else if (usage == GraphicsResource::DYNAMIC_UPDATE){
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }else{  // usage == Resource::SHADER_OUTPUT
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
        desc.CPUAccessFlags = 0;
    }

	//如果有mip map, 且可以被共享, 就可以render target, 用处也变为default
    if (texture->WantAutogenerateMipmaps() && !texture->IsShared()){
        desc.Usage = D3D11_USAGE_DEFAULT; //gpu可以读写
        desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
    }

    // Create the texture.
    ID3D11Texture2D* dxTexture = nullptr;
    HRESULT hr;
    if (texture->GetData()){
        int numSubresources = texture->GetNumSubresources();
        std::vector<D3D11_SUBRESOURCE_DATA> data(numSubresources);
        for (int index = 0; index < numSubresources; ++index){
            auto sr = texture->GetSubresource(index);
            data[index].pSysMem = sr.data;
            data[index].SysMemPitch = sr.rowPitch;
            data[index].SysMemSlicePitch = 0;
        }
        hr = device->CreateTexture2D(&desc, &data[0], &dxTexture);
    }else{
        hr = device->CreateTexture2D(&desc, nullptr, &dxTexture);
    }
    _dxObj = dxTexture;

    // Create views of the texture.
    CreateSRView(device, desc);
    if (texture->GetUsage() == GraphicsResource::SHADER_OUTPUT){
        CreateUAView(device, desc);
    }

    // Create a staging texture if requested.
    if (texture->GetCopyType() != GraphicsResource::COPY_NONE){
        CreateStaging(device, desc);
    }
}

DX11Texture2::DX11Texture2(ID3D11Device* device, DX11Texture2* dxSharedTexture)
    :DX11TextureSingle(dxSharedTexture->GetTexture()){
    ID3D11Texture2D* dxShared = dxSharedTexture->CreateSharedDXObject(device);
    _dxObj = dxShared;
    D3D11_TEXTURE2D_DESC desc;
    dxShared->GetDesc(&desc);
    CreateSRView(device, desc);

    if (dxSharedTexture->GetTexture()->GetUsage() == GraphicsResource::SHADER_OUTPUT){
        CreateUAView(device, desc);
    }
}

DX11Texture2* DX11Texture2::Create(ID3D11Device* device, GraphicsObject* object){
    if (object->GetType() == GT_TEXTURE2){
        return new DX11Texture2(device, static_cast<Texture2*>(object));
    }

    return nullptr;
}

DX11Texture2::DX11Texture2(Texture2* texture)
    :DX11TextureSingle(texture){
}

Texture2* DX11Texture2::GetTexture(){
    return static_cast<Texture2*>(_pluObj);
}

ID3D11Texture2D* DX11Texture2::GetDXTexture(){
    return static_cast<ID3D11Texture2D*>(_dxObj);
}

ID3D11Texture2D* DX11Texture2::CreateSharedDXObject(ID3D11Device* device){
    IDXGIResource* resource = nullptr;
    HRESULT hr = _dxObj->QueryInterface(__uuidof(IDXGIResource),
        (void**)&resource);
    //CHECK_HR_RETURN("QueryInterface failed", nullptr);

    HANDLE handle = nullptr;
    hr = resource->GetSharedHandle(&handle);
    resource->Release();
    //CHECK_HR_RETURN("GetSharedHandle failed", nullptr);
	
    ID3D11Texture2D* dxShared = nullptr;
    hr = device->OpenSharedResource(handle, __uuidof(ID3D11Texture2D),
        (void**)&dxShared);
    //CHECK_HR_RETURN("OpenSharedResource failed", nullptr);
    return dxShared;
}

void DX11Texture2::CreateStaging(ID3D11Device* device, D3D11_TEXTURE2D_DESC & tx){
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = tx.Width;
    desc.Height = tx.Height;
    desc.MipLevels = tx.MipLevels;
    desc.ArraySize = tx.ArraySize;
    desc.Format = tx.Format;
    desc.SampleDesc.Count = tx.SampleDesc.Count;
    desc.SampleDesc.Quality = tx.SampleDesc.Quality;
    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = msStagingAccess[GetTexture()->GetCopyType()];
    desc.MiscFlags = 0;

    HRESULT hr = device->CreateTexture2D(&desc, nullptr,
        reinterpret_cast<ID3D11Texture2D**>(&mStaging));
}

void DX11Texture2::CreateSRView(ID3D11Device* device, D3D11_TEXTURE2D_DESC& tx){
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    desc.Format = tx.Format;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MostDetailedMip = 0;
    desc.Texture2D.MipLevels = tx.MipLevels;

    HRESULT hr = device->CreateShaderResourceView(GetDXTexture(), &desc,
        &mSRView);
}

void DX11Texture2::CreateUAView(ID3D11Device* device, D3D11_TEXTURE2D_DESC& tx){
    D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
    desc.Format = tx.Format;
    desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice = 0;

    HRESULT hr = device->CreateUnorderedAccessView(GetDXTexture(), &desc,
        &mUAView);
}