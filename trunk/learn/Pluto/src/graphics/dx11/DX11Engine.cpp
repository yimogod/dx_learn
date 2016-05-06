#include <d3dcompiler.h>
#include <util/DDSTextureLoader.h>
#include <util/CUtil.h>

#include <graphics/dx11/DX11BlendState.h>
#include <graphics/dx11/DX11DepthStencilState.h>
#include <graphics/dx11/DX11RasterizerState.h>
#include <graphics/dx11/DX11Engine.h>

using namespace DirectX;
using namespace plu;

DX11Engine::DX11Engine(HWND hwnd){
	_hwnd = hwnd;
	init();
	createDevice();
	createRenderTargetlView();
	createDepthStencilView();
	createViewPort();
}

DX11Engine::~DX11Engine(){}

void DX11Engine::init(){
	RECT rect;
	GetClientRect(_hwnd, &rect);
	_width = rect.right - rect.left;
	_height = rect.bottom - rect.top;

	_resViewNum = 0;

	_device = nullptr;
	_context = nullptr;
	_chain = nullptr;

	_renderTargetBuffer = nullptr;
	_renderTargetView = nullptr;

	_depthStencilBuffer = nullptr;
	_depthStencilView = nullptr;


	_defaultRasterState = nullptr;
	_activeRasterState = nullptr;
	_defaultBlendState = nullptr;
	_activeBlendState = nullptr;
	_defaultDepthStencilState = nullptr;
	_activeDepthStencilState = nullptr;
}

void DX11Engine::destroy(){
	if(_vs)_vs->Release();
	if(_ps)_ps->Release();
	if(_vertexLayout)_vertexLayout->Release();
	if(_vertexBuff)_vertexBuff->Release();

	_vs = nullptr;
	_ps = nullptr;
	_vertexLayout = nullptr;
	_vertexBuff = nullptr;

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
}

bool DX11Engine::createDevice(){
	_driverType = D3D_DRIVER_TYPE_HARDWARE;
	int createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = S_OK;
	/*创建device*/
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _width;
	sd.BufferDesc.Height = _height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_BACK_BUFFER;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	//sd.SampleDesc.Count = 4;
	//sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	sd.Windowed = TRUE;
	sd.OutputWindow = _hwnd;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &_chain, &_device, &_featureLevel, &_context);
	if(FAILED(hr))return false;

	return true;
}

bool DX11Engine::createDepthStencilView(){
	/*声明深度模板描述数据*/
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = _width;
	td.Height = _height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	/*创建深度/模板缓存*/
	HRESULT hr = _device->CreateTexture2D(&td, nullptr, &_depthStencilBuffer);
	if(FAILED(hr))return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	hr = _device->CreateDepthStencilView(_depthStencilBuffer, &dsvd, &_depthStencilView);
	if(FAILED(hr))return false;
	_context->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	return true;
}

bool DX11Engine::createRenderTargetlView(){
	/*创建back buff*/
	_renderTargetBuffer = nullptr;
	HRESULT hr = _chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_renderTargetBuffer));
	if(FAILED(hr))return false;

	/*创建render target*/
	hr = _device->CreateRenderTargetView(_renderTargetBuffer, nullptr, &_renderTargetView);
	if(FAILED(hr))return false;
	return true;
}

void DX11Engine::createViewPort(){
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

void DX11Engine::createDefaultObjects(){
	_defaultBlendState = make_shared<BlendState>();
	_defaultDepthStencilState = make_shared<DepthStencilState>();
	_defaultRasterState = make_shared<RasterizerState>();

	setBlendState(_defaultBlendState);
	setDepthStencilState(_defaultDepthStencilState);
	setRasterizerState(_defaultRasterState);
}

void DX11Engine::setBlendState(shared_ptr<BlendState>& state){
	if(state == _activeBlendState)return;

	BlendState* bs = state.get();
	DX11BlendState* dx = DX11BlendState::create(_device, bs);
	if(dx){
		dx->enable(_context);
		_activeBlendState = state;
	}
}

shared_ptr<BlendState>& DX11Engine::getBlendState(){
	return _activeBlendState;
}

void DX11Engine::setDepthStencilState(shared_ptr<DepthStencilState>& state){
	if(state == _activeDepthStencilState)return;

	DepthStencilState* dss = state.get();
	DX11DepthStencilState* dx = DX11DepthStencilState::create(_device, dss);
	if(dx){
		dx->enable(_context);
		_activeDepthStencilState = state;
	}
}

shared_ptr<DepthStencilState>& DX11Engine::getDepthStencilState(){
	return _activeDepthStencilState;
}

void DX11Engine::setRasterizerState(shared_ptr<RasterizerState>& state){
	if(state == _activeDepthStencilState)return;

	RasterizerState* rs = state.get();
	DX11RasterizerState* dx = DX11RasterizerState::create(_device, rs);
	if(dx){
		dx->enable(_context);
		_activeRasterState = state;
	}
}

shared_ptr<RasterizerState>& DX11Engine::getRasterizerState(){
	return _activeRasterState;
}

HRESULT DX11Engine::compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint,
	LPCSTR szShaderModel, ID3DBlob** ppBlobOut){
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	//#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	//#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if(FAILED(hr) && pErrorBlob){
		OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
	}
	if(pErrorBlob){
		pErrorBlob->Release();
		return S_FALSE;
	}
	if(FAILED(hr)){
		return S_FALSE;
	}

	return S_OK;
}

bool DX11Engine::createVertexBuffer(Vertex *vertices, int vertNum, int vertSize){
	return createVertexBuffer(vertices, vertNum * vertSize, &_vertexBuff);
}

bool DX11Engine::createVertexBuffer(Vertex *vertices, int byteWidth, ID3D11Buffer** vertexBuff){
	/*设置buff desc*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWidth;//数据总长度
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = NULL;

	/*设置 resource data*/
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	/*创建vertex buff*/
	HRESULT hr = _device->CreateBuffer(&bd, &InitData, vertexBuff);
	if(FAILED(hr))return false;

	return true;
}

void DX11Engine::bindVertexBuff(){
	bindVertexBuff(_vertexBuff);
}

void DX11Engine::bindVertexBuff(ID3D11Buffer* vertexBuff){
	/*设置 layout*/
	_context->IASetInputLayout(_vertexLayout);

	/*设置当前vertex buff*/
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &vertexBuff, &stride, &offset);

	/*设置三角形排列方式, 即顶点拓扑方式*/
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*设置index buff*/
bool DX11Engine::createIndexBuffer(unsigned short* indexList, int indexNum){
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(short) * indexNum;//定点数据占用的所有空间
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = NULL;

	/*设置 resource data*/
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indexList;
	HRESULT hr = _device->CreateBuffer(&bd, &InitData, &_indexBuff);
	if(FAILED(hr))return false;
	return true;
}

void DX11Engine::bindIndexBuff(){
	_context->IASetIndexBuffer(_indexBuff, DXGI_FORMAT_R16_UINT, 0);
}

bool DX11Engine::createConstBuffer(ID3D11Buffer** constBuff, int byteWidth){
	/*创建constant buff, 类似于uniform变量*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = NULL;

	HRESULT hr = _device->CreateBuffer(&bd, nullptr, constBuff);
	if(FAILED(hr))return false;

	return true;
}

bool DX11Engine::createShader(CreateShaderInfo vs, CreateShaderInfo ps, D3D11_INPUT_ELEMENT_DESC layout[], int numElements){
	/*编译shader*/
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = compileShaderFromFile(vs.fileName, vs.entryPoint, vs.shaderModel, &pVSBlob);
	if(FAILED(hr)){
		pVSBlob->Release();
		return false;
	}

	/*创建 vertex shader*/
	hr = _device->CreateVertexShader(pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), nullptr, &_vs);

	if(FAILED(hr)){
		pVSBlob->Release();
		return false;
	}

	hr = _device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &_vertexLayout);
	pVSBlob->Release();
	if(FAILED(hr))return false;

	/*编译shader*/
	ID3DBlob* pPSBlob = nullptr;
	hr = compileShaderFromFile(ps.fileName, ps.entryPoint, ps.shaderModel, &pPSBlob);
	if(FAILED(hr)){
		pPSBlob->Release();
		return false;
	}

	/*创建 pixel shader*/
	hr = _device->CreatePixelShader(pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(), nullptr, &_ps);
	pPSBlob->Release();

	if(FAILED(hr))return false;
	return true;
}

bool DX11Engine::createDepthState(){
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

bool DX11Engine::createRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs){
	D3D11_RASTERIZER_DESC rsd;
	ZeroMemory(&rsd, sizeof(D3D11_RASTERIZER_DESC));

	rsd.FillMode = fillmode;
	rsd.CullMode = D3D11_CULL_BACK;
	//rsd.CullMode = D3D11_CULL_FRONT;
	//rsd.CullMode = D3D11_CULL_NONE;
	rsd.FrontCounterClockwise = false;
	rsd.DepthClipEnable = true;
	rsd.DepthBias = 0;
	rsd.DepthBiasClamp = 0.0f;
	rsd.ScissorEnable = false;

	HRESULT hr = _device->CreateRasterizerState(&rsd, &rs);
	if(FAILED(hr))return false;

	_context->RSSetState(rs);
	return true;
}

void DX11Engine::createAlphaBlendState(){
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

void DX11Engine::enableAlphaBlend(){
	float blendFactor[4] = { 0, 0, 0, 0 };
	_context->OMSetBlendState(_blendEnableState, blendFactor, D3D11_DEFAULT_SAMPLE_MASK);
}

void DX11Engine::disableAlphaBlend(){
	float blendFactor[4] = { 0, 0, 0, 0 };
	_context->OMSetBlendState(_blendDisableState, blendFactor, D3D11_DEFAULT_SAMPLE_MASK);
}

bool DX11Engine::createTexture(const wchar_t* path){
	HRESULT hr = CreateDDSTextureFromFile(_device, path, nullptr, &_resView[_resViewNum], 2048U);
	if(FAILED(hr))return false;
	_resViewNum++;
	return true;
}

// Create the sample state
bool DX11Engine::createSamplerState(){
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
