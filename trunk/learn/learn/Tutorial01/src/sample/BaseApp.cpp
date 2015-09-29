#include <d3dcompiler.h>
#include "BaseApp.h"

BaseApp::BaseApp():_driverType(D3D_DRIVER_TYPE_NULL),
				_featureLevel(D3D_FEATURE_LEVEL_11_0),
				_device(NULL),
				_context(NULL),
				_chain(NULL),
				_backBuffTarget(NULL)
{}

BaseApp::~BaseApp(){
	destroy();
}

bool BaseApp::init(HINSTANCE ins, HWND hwnd){
	_ins = ins;
	_hwnd = hwnd;

	RECT rect;
	GetClientRect(_hwnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	int createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	/*创建swap chain desc*/
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hwnd;
	sd.Windowed = true;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;


	HRESULT hr = S_OK;
	/*创建device and chain*/
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr,
		createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &_chain,
		&_device, &_featureLevel, &_context);
	if(FAILED(hr))return hr;

	/*创建back buff*/
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if(FAILED(hr))return hr;

	/*创建render target*/
	hr = _device->CreateRenderTargetView(pBackBuffer, nullptr, &_backBuffTarget);
	pBackBuffer->Release();
	if(FAILED(hr))return hr;

	_context->OMSetRenderTargets(1, &_backBuffTarget, nullptr);

	/*设置viewport*/
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_context->RSSetViewports(1, &vp);

	return loadContent();
}

bool BaseApp::loadContent(){
	return true;
}

void BaseApp::unloadContent(){

}

void BaseApp::render(){

}

void BaseApp::update(){

}

void BaseApp::destroy(){
	unloadContent();

	if(_backBuffTarget)_backBuffTarget->Release();
	if(_chain)_chain->Release();
	if(_context)_context->Release();
	if(_device)_device->Release();

	_backBuffTarget = NULL;
	_chain = NULL;
	_context = NULL;
	_device = NULL;
}

HRESULT BaseApp::compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint,
							LPCSTR szShaderModel, ID3DBlob** ppBlobOut){
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	
	if(FAILED(hr) && pErrorBlob){
		OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
	}
	if(pErrorBlob) pErrorBlob->Release();

	return S_OK;
}