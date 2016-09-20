#include <graphics/DXEngine.h>
#include <util/DDSTextureLoader.h>
#include <BaseDataStruct.h>

using namespace DirectX;

DXEngine::DXEngine():
	_driverType(D3D_DRIVER_TYPE_HARDWARE),
	_featureLevel(D3D_FEATURE_LEVEL_11_1)
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
	//����device, ѧϰ����, ֻ֧��11.1. ������ѧϰdx12��ʱ��, �����dx12
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferDesc.Width = _width;
	sd.BufferDesc.Height = _height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	
	sd.SampleDesc.Count = 1;//�����Ҫ���õĸ�depth sample desc countһ��, ������Ⱦ������!!!
	sd.SampleDesc.Quality = 0;//9600����Ϊ1�Ļ�����

	sd.BufferCount = 1; //���ٸ��󻺳���, ���˫����Ļ�, ����Ϊ1
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.Windowed = TRUE;
	sd.OutputWindow = _hwnd;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &_chain, &_device, &_featureLevel, &_context);
	if(FAILED(hr))return false;
	return true;
}

void DXEngine::InitVisual(DXVisual &visual, void* vertices, int vertexNum, int* indices, int indexNum){
	visual.Init(_device, vertices, vertexNum, indices, indexNum);
}

void DXEngine::InitDevice(HWND const &hwnd, int screenWidth, int screenHeight){
	CreateDevice(hwnd, screenWidth, screenHeight);
	CreateDepthStencilView();
	CreateRenderTargetlView();
	SetDefaultRenderTargetView();
	CreateViewPort();

	CreateDepthState();
	CreateRasterizerState(D3D11_FILL_MODE::D3D11_FILL_SOLID, _solidRS);
	CreateAlphaBlendState();

	_ready = true;
}

bool DXEngine::CreateDepthStencilView(){
	//�������ģ��<����>����
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = _width;
	td.Height = _height;
	td.MipLevels = 1; //��Ȼ��治��Ҫmip level
	td.ArraySize = 1; //texture array��texture�ĸ���, �����Ȼ���, 1��
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.Usage = D3D11_USAGE_DEFAULT;

	td.SampleDesc.Count = 1;//���ֻ������Ϊ1, �������
	td.SampleDesc.Quality = 0;

	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	/*�������/ģ�建��*/
	HRESULT hr = _device->CreateTexture2D(&td, nullptr, &_depthStencilBuffer);
	if(FAILED(hr))return false;

	//��Ȼ�������
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	hr = _device->CreateDepthStencilView(_depthStencilBuffer, &dsvd, &_depthStencilView);
	if(FAILED(hr))return false;
	return true;
}

//����ֻ�Ǵ�����render target view, ��û�����õ�context��--OMSetRenderTargets
bool DXEngine::CreateRenderTargetlView(){
	//����back buff
	_renderTargetBuffer = nullptr;
	HRESULT hr = _chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_renderTargetBuffer));
	if(FAILED(hr))return false;

	//����render target
	hr = _device->CreateRenderTargetView(_renderTargetBuffer, nullptr, &_renderTargetView);
	if(FAILED(hr))return false;

	return true;
}

void DXEngine::CreateViewPort(){
	/*����viewport*/
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




bool DXEngine::CreateDepthState(){
	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

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
	rsd.CullMode = D3D11_CULL_BACK;//���������޳�
	//rsd.CullMode = D3D11_CULL_FRONT;
	//rsd.CullMode = D3D11_CULL_NONE;
	rsd.FrontCounterClockwise = false; //˳ʱ��Ϊfalse, ����������ʱ��Ϊ������
	rsd.DepthClipEnable = true; //��ȼ��п���
	rsd.ScissorEnable = false;
	rsd.DepthBias = 0; //
	rsd.DepthBiasClamp = 0.0f;

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

	// ����һ��alpha blend״̬.
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

void DXEngine::ClearBuffers(const FLOAT ColorRGBA[4]){
	_context->ClearRenderTargetView(_renderTargetView, ColorRGBA);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}