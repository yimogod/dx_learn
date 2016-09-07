#include <graphics/DXEngine.h>
#include <util/DDSTextureLoader.h>
#include <BaseDataStruct.h>

using namespace DirectX;

DXEngine::DXEngine():
	_driverType(D3D_DRIVER_TYPE_HARDWARE),
	_featureLevel(D3D_FEATURE_LEVEL_11_1),
	_resViewNum(0)
{
}

DXEngine::~DXEngine(){
}

bool DXEngine::CreateDevice(HWND const &hwnd, int screenWidth, int screenHeight){
	_hwnd = hwnd;
	_width = screenWidth;
	_height = screenHeight;

	_driverType = D3D_DRIVER_TYPE_HARDWARE;
	int createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = S_OK;
	//创建device, 学习代码, 只支持11.1. 另外在学习dx12的时候, 会加入dx12
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferDesc.Width = _width;
	sd.BufferDesc.Height = _height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	
	sd.SampleDesc.Count = 1;//这边需要设置的跟depth sample desc count一样, 否则渲染不出来!!!
	sd.SampleDesc.Quality = 0;//9600设置为1的话出错

	sd.BufferCount = 1; //多少个后缓冲区, 如果双缓冲的话, 设置为1
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.Windowed = TRUE;
	sd.OutputWindow = _hwnd;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &_chain, &_device, &_featureLevel, &_context);
	if(FAILED(hr))return false;
	return true;
}

void DXEngine::InitDevice(HWND const &hwnd, int screenWidth, int screenHeight){
	CreateDevice(hwnd, screenWidth, screenHeight);
	CreateDepthStencilView();
	CreateRenderTargetlView();
	_context->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
	CreateViewPort();

	CreateSamplerState();
	CreateDepthState();

	_ready = true;
}

bool DXEngine::CreateDepthStencilView(){
	//声明深度模板<数据>描述
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = _width;
	td.Height = _height;
	td.MipLevels = 1; //深度缓存不需要mip level
	td.ArraySize = 1; //texture array中texture的个数, 如果深度缓存, 1个
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.Usage = D3D11_USAGE_DEFAULT;

	td.SampleDesc.Count = 1;//这边只能设置为1, 否则出错
	td.SampleDesc.Quality = 0;

	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	/*创建深度/模板缓存*/
	HRESULT hr = _device->CreateTexture2D(&td, nullptr, &_depthStencilBuffer);
	if(FAILED(hr))return false;

	//深度缓存描述
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	hr = _device->CreateDepthStencilView(_depthStencilBuffer, &dsvd, &_depthStencilView);
	if(FAILED(hr))return false;
	return true;
}

//这里只是创建了render target view, 并没有设置到context中--OMSetRenderTargets
bool DXEngine::CreateRenderTargetlView(){
	//创建back buff
	_renderTargetBuffer = nullptr;
	HRESULT hr = _chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_renderTargetBuffer));
	if(FAILED(hr))return false;

	//创建render target
	hr = _device->CreateRenderTargetView(_renderTargetBuffer, nullptr, &_renderTargetView);
	if(FAILED(hr))return false;

	return true;
}

bool DXEngine::CreateRenderTargetViewByShaderRes(){
	HRESULT hr;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	/*创建后缓存视图用到的texture2d buff*/
	hr = _device->CreateTexture2D(&textureDesc, NULL, &_renderTargetBuffer);
	if(FAILED(hr))return false;

	/*创建render target view*/
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = _device->CreateRenderTargetView(_renderTargetBuffer, &renderTargetViewDesc, &_renderTargetView);
	if(FAILED(hr))return false;

	/*创建于shader关联的贴图资源, 跟render target 的缓存关联*/
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc;
	shaderResViewDesc.Format = textureDesc.Format;
	shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResViewDesc.Texture2D.MipLevels = 1;

	hr = _device->CreateShaderResourceView(_renderTargetBuffer, &shaderResViewDesc, &_renderTargetResView);
	if(FAILED(hr))return false;

	return true;
}


void DXEngine::CreateViewPort(){
	/*设置viewport*/
	D3D11_VIEWPORT vp;
	vp.Width = _width;
	vp.Height = _height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_context->RSSetViewports(1, &vp);
}

/*void DXEngine::Destroy(){
	unloadContent();

	if(_depthStencilBuffer)_depthStencilBuffer->Release();
	if(_depthStencilView)_depthStencilView->Release();
	if(_renderTargetBuffer)_renderTargetBuffer->Release();
	if(_renderTargetView)_renderTargetView->Release();
	if(_chain)_chain->Release();
	if(_context)_context->Release();
	if(_device)_device->Release();

	_depthStencilBuffer = NULL;
	_depthStencilView = NULL;
	_renderTargetBuffer = NULL;
	_renderTargetView = NULL;
	_chain = NULL;
	_context = NULL;
	_device = NULL;
}*/



bool DXEngine::CreateVertexBuffer(Vertex *vertices, int vertNum, int vertSize){
	return CreateVertexBuffer(vertices, vertNum * vertSize, &_vertexBuff);
}


bool DXEngine::CreateShader(VertexShader &vs, PixelShader &ps, InputLayout& layout){
	/*创建 vertex shader*/
	bool result = vs.CreateVertexShader(_device, layout);
	if(!result)return false;

	/*创建 pixel shader*/
	result = ps.CreatePixelShader(_device);
	return result;
}

bool DXEngine::CreateDepthState(){
	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	dsd.StencilEnable = false;
	dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace = dsd.FrontFace;

	HRESULT hr = _device->CreateDepthStencilState(&dsd, &_depthStencilState);
	if(FAILED(hr))return false;

	_context->OMSetDepthStencilState(_depthStencilState, 0);
	return true;
}

bool DXEngine::CreateRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs){
	D3D11_RASTERIZER_DESC rsd;
	ZeroMemory(&rsd, sizeof(D3D11_RASTERIZER_DESC));

	rsd.FillMode = fillmode;
	rsd.CullMode = D3D11_CULL_BACK;//开启背面剔除
	//rsd.CullMode = D3D11_CULL_FRONT;
	//rsd.CullMode = D3D11_CULL_NONE;
	rsd.FrontCounterClockwise = false; //逆时针为false, 及三角形顺时针为正方向
	rsd.DepthClipEnable = true; //深度剪切开启
	rsd.DepthBias = 0; //
	rsd.DepthBiasClamp = 0.0f;
	rsd.ScissorEnable = false;

	HRESULT hr = _device->CreateRasterizerState(&rsd, &rs);
	if(FAILED(hr))return false;

	_context->RSSetState(rs);
	return true;
}

void DXEngine::CreateAlphaBlendState(){
	D3D11_BLEND_DESC bsr;
	ZeroMemory(&bsr, sizeof(D3D11_BLEND_DESC));
	//bsr.AlphaToCoverageEnable = false;
	//bsr.IndependentBlendEnable = false;

	// 创建一个alpha blend状态.
	bsr.RenderTarget[0].BlendEnable = TRUE;
	bsr.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsr.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsr.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	bsr.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bsr.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsr.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bsr.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//0x0f;

	_device->CreateBlendState(&bsr, &_blendEnableState);

	bsr.RenderTarget[0].BlendEnable = FALSE;
	_device->CreateBlendState(&bsr, &_blendDisableState);
}

void DXEngine::EnableAlphaBlend(){
	float blendFactor[4] = { 0, 0, 0, 0 };
	_context->OMSetBlendState(_blendEnableState, blendFactor, D3D11_DEFAULT_SAMPLE_MASK);
}

void DXEngine::DisableAlphaBlend(){
	float blendFactor[4] = { 0, 0, 0, 0 };
	_context->OMSetBlendState(_blendDisableState, blendFactor, D3D11_DEFAULT_SAMPLE_MASK);
}

bool DXEngine::CreateTexture(const wchar_t* path){
	HRESULT hr = CreateDDSTextureFromFile(_device, path, nullptr, &_resView[_resViewNum], 2048U);
	if(FAILED(hr))return false;
	_resViewNum++;
	return true;
}

// Create the sample state
bool DXEngine::CreateSamplerState(){
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = _device->CreateSamplerState(&sampDesc, &_sampleState);
	if(FAILED(hr))return false;

	return true;
}

void DXEngine::ClearBuffers(const FLOAT ColorRGBA[4]){
	_context->ClearRenderTargetView(_renderTargetView, ColorRGBA);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DXEngine::Present(){
	_chain->Present(0, 0);
}