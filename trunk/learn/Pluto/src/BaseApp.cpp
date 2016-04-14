#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include <DDSTextureLoader.h>
#include "BaseApp.h"

#pragma comment (lib, "libfbxsdk.lib")

#define DIRECTINPUT_VERSION 0x0800

using namespace DirectX;

BaseApp::BaseApp():_driverType(D3D_DRIVER_TYPE_NULL),
				_featureLevel(D3D_FEATURE_LEVEL_11_1),
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
	_width = rect.right - rect.left;
	_height = rect.bottom - rect.top;

	return loadContent();
}

bool BaseApp::createDevice(){
	_driverType = D3D_DRIVER_TYPE_HARDWARE;
	int createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = S_OK;
	/*创建device*/
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	hr = D3D11CreateDevice(nullptr, _driverType, nullptr,
		createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &_device, &_featureLevel, &_context);
	if(FAILED(hr))return false;

	/*这里可以启动多重采样*/
	UINT m4xMsaaQuality;
	_device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);

	/*开始创建swap chain需要的factory*/
	IDXGIDevice* dxgiDevice = nullptr;
	hr = _device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	if(FAILED(hr)){
		dxgiDevice->Release();
		return false;
	}

	IDXGIAdapter* adapter = nullptr;
	hr = dxgiDevice->GetAdapter(&adapter);
	if(FAILED(hr)){
		adapter->Release();
		dxgiDevice->Release();
		return false;
	}

	IDXGIFactory1* dxgiFactory = nullptr;
	hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
	adapter->Release();
	dxgiDevice->Release();
	if(FAILED(hr))return false;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _width;
	sd.BufferDesc.Height = _height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	//sd.SampleDesc.Count = 4;
	//sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	sd.Windowed = TRUE;
	sd.OutputWindow = _hwnd;
	//sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	hr = dxgiFactory->CreateSwapChain(_device, &sd, &_chain);
	dxgiFactory->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);
	dxgiFactory->Release();

	if(FAILED(hr))return false;

	/*创建back buff*/
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if(FAILED(hr))return false;

	/*创建render target*/
	hr = _device->CreateRenderTargetView(pBackBuffer, nullptr, &_backBuffTarget);
	pBackBuffer->Release();
	if(FAILED(hr))return hr;

	/*声明深度模板描述数据*/
	D3D11_TEXTURE2D_DESC td;
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
	hr = _device->CreateTexture2D(&td, 0, &_depthStencilBuffer);
	if(FAILED(hr))return hr;
	hr = _device->CreateDepthStencilView(_depthStencilBuffer, 0, &_depthStencilView);
	if(FAILED(hr))return hr;

	//_context->OMSetRenderTargets(1, &_backBuffTarget, _depthStencilView);
	_context->OMSetRenderTargets(1, &_backBuffTarget, __nullptr);

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

bool BaseApp::loadContent(){
	return true;
}

void BaseApp::unloadContent(){
	if(_mouse){
		_mouse->Unacquire();
		_mouse->Release();
	}
	if(_keyborad){
		_keyborad->Unacquire();
		_keyborad->Release();
	}
	if(_inputDevice)_inputDevice->Release();
	if(_vs)_vs->Release();
	if(_ps)_ps->Release();
	if(_vertexLayout)_vertexLayout->Release();
	if(_vertexBuff)_vertexBuff->Release();

	_mouse = nullptr;
	_keyborad = nullptr;
	_inputDevice = nullptr;
	_vs = nullptr;
	_ps = nullptr;
	_vertexLayout = nullptr;
	_vertexBuff = nullptr;
}

void BaseApp::render(){
}

void BaseApp::update(){
	
}

void BaseApp::acquireInput(){
	HRESULT hr = S_OK;
	if(_keyborad != NULL){
		/*按下键盘*/
		hr = _keyborad->Acquire();
		if(SUCCEEDED(hr)){
			hr = _keyborad->GetDeviceState(sizeof(_keyboardBuff), (LPVOID)&_keyboardBuff);
		}
	}

	if(_mouse != NULL){
		/*按下鼠标*/
		hr = _mouse->Acquire();
		if(SUCCEEDED(hr)){
			hr = _mouse->GetDeviceState(sizeof(MouseState), &_mouseState);
			if(SUCCEEDED(hr)){
				//i want a simple ui
			}
		}
	}
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
	if(pErrorBlob){
		pErrorBlob->Release();
		return S_FALSE;
	}
	if(FAILED(hr)){
		return S_FALSE;
	}

	return S_OK;
}

bool BaseApp::createVertexBuffer(Vertex *vertices, int vertNum){
	/*设置buff desc*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertNum;//数据总长度
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = NULL;

	/*设置 resource data*/
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	/*创建vertex buff*/
	HRESULT hr = _device->CreateBuffer(&bd, &InitData, &_vertexBuff);
	if(FAILED(hr))return false;

	/*设置当前vertex buff*/
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_vertexBuff, &stride, &offset);

	/*设置三角形排列方式, 即顶点拓扑方式*/
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

/*设置index buff*/
bool BaseApp::createIndexBuffer(unsigned short* indexList, int indexNum){
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

	_context->IASetIndexBuffer(_indexBuff, DXGI_FORMAT_R16_UINT, 0);

	return true;
}

bool BaseApp::createConstBuffer(ID3D11Buffer** constBuff, int byteWidth){
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

bool BaseApp::createShader(CreateShaderInfo vs, CreateShaderInfo ps, D3D11_INPUT_ELEMENT_DESC layout[], int numElements){
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

	/*设置 layout*/
	_context->IASetInputLayout(_vertexLayout);

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

bool BaseApp::createRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs){
	D3D11_RASTERIZER_DESC rsd;
	ZeroMemory(&rsd, sizeof(D3D11_RASTERIZER_DESC));

	rsd.FillMode = fillmode;
	rsd.CullMode = D3D11_CULL_NONE;
	rsd.FrontCounterClockwise = false;
	rsd.DepthClipEnable = true;

	HRESULT hr = _device->CreateRasterizerState(&rsd, &rs);
	if(FAILED(hr))return false;

	_context->RSSetState(rs);
	return true;
}

bool BaseApp::createTexture(const wchar_t* path){
	HRESULT hr = CreateDDSTextureFromFile(_device, path, nullptr, &_resView, 2048U);
	if(FAILED(hr))return false;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = _device->CreateSamplerState(&sampDesc, &_sampleState);
	if(FAILED(hr))return false;

	return true;
}

bool BaseApp::createDXInput(){
	ZeroMemory(_keyboardBuff, sizeof(_keyboardBuff));

	HRESULT hr = DirectInput8Create(_ins, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&_inputDevice, nullptr);
	if(FAILED(hr))return false;

	/*创建鼠标*/
	hr = _inputDevice->CreateDevice(GUID_SysMouse, &_mouse, nullptr);
	if(FAILED(hr))return false;


	_inputObjFormat[0] = { &GUID_XAxis, FIELD_OFFSET(MouseState, lAxisX),    // X axis
		DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 };
	_inputObjFormat[1] = { &GUID_YAxis, FIELD_OFFSET(MouseState, lAxisY),    // Y axis
		DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 };
	_inputObjFormat[2] = { 0, (DWORD)FIELD_OFFSET(MouseState, abButtons[0]),        // Button 0
		DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 };
	_inputObjFormat[3] = { 0, (DWORD)FIELD_OFFSET(MouseState, abButtons[1]),        // Button 1 (optional)
		DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 };
	_inputObjFormat[4] = { 0, (DWORD)FIELD_OFFSET(MouseState, abButtons[2]),        // Button 2 (optional)
		DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 };

	DWORD numMouseObjects = 5;
	_mouseDataFormat = {
		sizeof(DIDATAFORMAT),
		sizeof(DIOBJECTDATAFORMAT),
		DIDF_ABSAXIS,
		sizeof(MouseState),
		numMouseObjects,
		_inputObjFormat
	};

	hr = _mouse->SetDataFormat(&_mouseDataFormat);
	if(FAILED(hr))return false;

	hr = _mouse->SetCooperativeLevel(_hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if(FAILED(hr))return false;

	hr = _mouse->Acquire();
	if(FAILED(hr))return false;

	/*创建键盘*/
	hr = _inputDevice->CreateDevice(GUID_SysKeyboard, &_keyborad, nullptr);
	if(FAILED(hr))return false;

	hr = _keyborad->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))return false;

	hr = _keyborad->SetCooperativeLevel(_hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if(FAILED(hr))return false;

	return true;
}

bool BaseApp::isKeyDown(char keycode){
	char a = _keyboardBuff[keycode];
	return a & 0x80;
}

void BaseApp::UpdatePosByKeyboard(Camera* camera, float value){
	acquireInput();

	if(isKeyDown(DIK_A)){
		camera->position.x -= value;
	}else if(isKeyDown(DIK_D)){
		camera->position.x += value;
	}else if(isKeyDown(DIK_W)){
		camera->position.y += value;
	}else if(isKeyDown(DIK_S)){
		camera->position.y -= value;
	}else if(isKeyDown(DIK_Q)){
		camera->position.z += value;
	}else if(isKeyDown(DIK_E)){
		camera->position.z -= value;
	}else if(isKeyDown(DIK_J)){
		camera->rotateY -= value;
	}else if(isKeyDown(DIK_L)){
		camera->rotateY += value;
	}else if(isKeyDown(DIK_I)){
		camera->rotateX -= value;
	}else if(isKeyDown(DIK_K)){
		camera->rotateX += value;
	}else if(isKeyDown(DIK_U)){
		camera->rotateZ -= value;
	}else if(isKeyDown(DIK_O)){
		camera->rotateZ += value;
	}
}