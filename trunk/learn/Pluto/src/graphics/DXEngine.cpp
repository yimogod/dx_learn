#include <graphics/DXEngine.h>
#include <util/DDSTextureLoader.h>
#include <BaseDataStruct.h>

using namespace DirectX;

DXEngine::DXEngine():
	_driverType(D3D_DRIVER_TYPE_HARDWARE),
	_featureLevel(D3D_FEATURE_LEVEL_11_1)
{
}

DXEngine::~DXEngine(){}

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

void DXEngine::InitVisual(DXVisual &visual, void* vertices, int vertexNum, int* indices, int indexNum){
	visual.Init(_device, vertices, vertexNum, indices, indexNum);
}

void DXEngine::InitDevice(HWND const &hwnd, int screenWidth, int screenHeight){
	CreateDevice(hwnd, screenWidth, screenHeight);
	_defaultDepthState.CreateDepthView(_device, screenWidth, screenHeight);
	CreateRenderTargetlView();
	SetDefaultRenderTargetView();
	CreateViewPort();

	_defaultDepthState.CreateDepthState(_device);
	CreateRasterizerState(D3D11_FILL_MODE::D3D11_FILL_SOLID, _solidRS);
	CreateAlphaBlendState();

	_ready = true;
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

	if(_renderTargetBuffer)_renderTargetBuffer->Release();
	if(_renderTargetView)_renderTargetView->Release();
	if(_chain)_chain->Release();
	if(_context)_context->Release();
	if(_device)_device->Release();

	_renderTargetBuffer = NULL;
	_renderTargetView = NULL;
	_chain = NULL;
	_context = NULL;
	_device = NULL;
}*/

void DXEngine::CreateRTT(int width, int height){
	if(width == 0 || height == 0){
		width = _width;
		height = _height;
	}
	_renderTexture.CreateRenderTargetView(_device, width, height);
}

bool DXEngine::CreateRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs){
	D3D11_RASTERIZER_DESC rsd;
	ZeroMemory(&rsd, sizeof(D3D11_RASTERIZER_DESC));

	rsd.FillMode = fillmode;
	rsd.CullMode = D3D11_CULL_BACK;//开启背面剔除
	//rsd.CullMode = D3D11_CULL_FRONT;
	//rsd.CullMode = D3D11_CULL_NONE;
	rsd.FrontCounterClockwise = false; //顺时针为false, 及三角形逆时针为正方向
	rsd.DepthClipEnable = true; //深度剪切开启
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
	bsr.AlphaToCoverageEnable = FALSE;
	bsr.IndependentBlendEnable = FALSE;

	// 创建一个alpha blend状态.
	bsr.RenderTarget[0].BlendEnable = TRUE;
	bsr.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsr.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsr.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	bsr.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bsr.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsr.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bsr.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//0x0f;

	HRESULT hr = _device->CreateBlendState(&bsr, &_blendEnableState);
	if(FAILED(hr))return;

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
	_defaultDepthState.SetDepthState(_context);
	_context->ClearRenderTargetView(_renderTargetView, ColorRGBA);
	_defaultDepthState.ClearDepthStencilView(_context);
}