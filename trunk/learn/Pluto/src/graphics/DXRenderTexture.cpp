#include <graphics\DXRenderTexture.h>

DXRenderTexture::DXRenderTexture(){
}

DXRenderTexture::~DXRenderTexture(){
}

bool DXRenderTexture::CreateRenderTargetView(ID3D11Device* device, int width, int height){
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	td.Usage = D3D11_USAGE_DEFAULT;

	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;

	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	/*创建后缓存视图用到的texture2d buff*/
	HRESULT hr = device->CreateTexture2D(&td, NULL, &_renderTargetBuffer);
	if(FAILED(hr))return false;

	/*创建render target view*/
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = td.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = device->CreateRenderTargetView(_renderTargetBuffer, &renderTargetViewDesc, &_renderTargetView);
	if(FAILED(hr))return false;

	/*创建于shader关联的贴图资源, 跟render target 的缓存关联*/
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc;
	shaderResViewDesc.Format = td.Format;
	shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResViewDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(_renderTargetBuffer, &shaderResViewDesc, &_renderTargetResView);
	if(FAILED(hr))return false;


	bool result = _depthState.CreateDepthView(device, width, height);
	if(!result)return false;

	result = _depthState.CreateDepthState(device, true);
	if(!result)return false;

	return true;
}

//rtt自带了一个深度缓冲区, 切换rtt时需要清理现有的缓冲区
void DXRenderTexture::ClearRTT(ID3D11DeviceContext* context){
	_depthState.SetDepthState(context);

	float color[4];
	color[0] = 0.501960814f;
	color[1] = 0.0f;
	color[2] = 0.501960814f;
	color[3] = 1.0f;
	context->ClearRenderTargetView(_renderTargetView, color);
	_depthState.ClearDepthStencilView(context);
}