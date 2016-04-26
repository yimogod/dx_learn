#include <d3dcompiler.h>
#include <dinput.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <util/DDSTextureLoader.h>
#include <util/CUtil.h>
#include "BaseApp.h"

#pragma comment (lib, "libfbxsdk.lib")
//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")
//#define DIRECTINPUT_VERSION 0x0800

using namespace DirectX;

BaseApp::BaseApp():_driverType(D3D_DRIVER_TYPE_NULL),
				_featureLevel(D3D_FEATURE_LEVEL_11_1),
				_device(NULL),
				_context(NULL),
				_chain(NULL),
				_backBuffView(NULL){}

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

	_resViewNum = 0;

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
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	//sd.SampleDesc.Count = 4;
	//sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	sd.Windowed = TRUE;
	sd.OutputWindow = _hwnd;
	//sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &_chain, &_device, &_featureLevel, &_context);
	if(FAILED(hr))return false;



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
	hr = _device->CreateTexture2D(&td, nullptr, &_depthStencilBuffer);
	if(FAILED(hr))return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	hr = _device->CreateDepthStencilView(_depthStencilBuffer, &dsvd, &_depthStencilView);
	if(FAILED(hr))return false;

	/*创建back buff*/
	_backBuffer = nullptr;
	hr = _chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_backBuffer));
	if(FAILED(hr))return false;

	/*创建render target*/
	hr = _device->CreateRenderTargetView(_backBuffer, nullptr, &_backBuffView);
	if(FAILED(hr))return false;

	//_context->OMSetRenderTargets(1, &_backBuffView, _depthStencilView);
	_context->OMSetRenderTargets(1, &_backBuffView, 0);

	/*设置viewport*/
	D3D11_VIEWPORT vp;
	vp.Width = _width;
	vp.Height = _height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_context->RSSetViewports(1, &vp);

	return true;
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
			hr = _mouse->GetDeviceState(sizeof(MouseState), (LPVOID)&_mouseState);
			if(SUCCEEDED(hr)){
				//i want a simple ui
			}
		}
	}
}

void BaseApp::destroy(){
	unloadContent();

	if(_depthStencilBuffer)_depthStencilBuffer->Release();
	if(_depthStencilView)_depthStencilView->Release();
	if(_backBuffer)_backBuffer->Release();
	if(_backBuffView)_backBuffView->Release();
	if(_chain)_chain->Release();
	if(_context)_context->Release();
	if(_device)_device->Release();

	_depthStencilBuffer = NULL;
	_depthStencilView = NULL;
	_backBuffer = NULL;
	_backBuffView = NULL;
	_chain = NULL;
	_context = NULL;
	_device = NULL;
}

HRESULT BaseApp::compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint,
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

bool BaseApp::createVertexBuffer(Vertex *vertices, int vertNum, int vertSize){
	/*设置buff desc*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertSize * vertNum;//数据总长度
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = NULL;

	/*设置 resource data*/
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	/*创建vertex buff*/
	HRESULT hr = _device->CreateBuffer(&bd, &InitData, &_vertexBuff);
	if(FAILED(hr))return false;

	return true;
}

void BaseApp::bindVertexBuff(){
	/*设置 layout*/
	_context->IASetInputLayout(_vertexLayout);

	/*设置当前vertex buff*/
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_vertexBuff, &stride, &offset);

	/*设置三角形排列方式, 即顶点拓扑方式*/
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	return true;
}

void BaseApp::bindIndexBuff(){
	_context->IASetIndexBuffer(_indexBuff, DXGI_FORMAT_R16_UINT, 0);
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
	rsd.CullMode = D3D11_CULL_BACK;
	//rsd.CullMode = D3D11_CULL_FRONT;
	//rsd.CullMode = D3D11_CULL_NONE;
	rsd.FrontCounterClockwise = true;
	rsd.DepthClipEnable = true;
	rsd.DepthBias = 0;
	rsd.DepthBiasClamp = 0.0f;

	HRESULT hr = _device->CreateRasterizerState(&rsd, &rs);
	if(FAILED(hr))return false;

	_context->RSSetState(rs);
	return true;
}

bool BaseApp::createBlendState(){
	D3D11_BLEND_DESC bsr;
	ZeroMemory(&bsr, sizeof(D3D11_BLEND_DESC));

	// 创建一个alpha blend状态.
	bsr.RenderTarget[0].BlendEnable = TRUE;
	bsr.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsr.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsr.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bsr.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bsr.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsr.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bsr.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//0x0f;

	HRESULT hr = _device->CreateBlendState(&bsr, &_blendState);
	if(FAILED(hr))return false;

	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	_context->OMSetBlendState(_blendState, blendFactor, D3D11_DEFAULT_SAMPLE_MASK);
	return true;
}

bool BaseApp::createTexture(const wchar_t* path){
	HRESULT hr = CreateDDSTextureFromFile(_device, path, nullptr, &_resView[_resViewNum], 2048U);
	if(FAILED(hr))return false;
	_resViewNum++;
	return true;
}

bool BaseApp::createRenderTargetWithShaderResView(){
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
	hr = _device->CreateTexture2D(&textureDesc, NULL, &_backBuffer);
	if(FAILED(hr))return false;

	/*创建render target view*/
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = _device->CreateRenderTargetView(_backBuffer, &renderTargetViewDesc, &_backBuffView);
	if(FAILED(hr))return false;

	/*创建于shader关联的贴图资源, 跟render target 的缓存关联*/
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc;
	shaderResViewDesc.Format = textureDesc.Format;
	shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResViewDesc.Texture2D.MipLevels = 1;

	hr = _device->CreateShaderResourceView(_backBuffer, &shaderResViewDesc, &_backBuffResView);
	if(FAILED(hr))return false;

	return true;
}

// Create the sample state
bool BaseApp::createSamplerState(){
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = _device->CreateSamplerState(&sampDesc, &_sampleState);
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
	return (a & 0x80) != 0;
}

bool BaseApp::isLMouseDown(){
	char a = _mouseState.abButtons[0];
	return (a & 0x80) != 0;
}

bool BaseApp::isRMouseDown(){
	char a = _mouseState.abButtons[1];
	return (a & 0x80) != 0;
}


/*根据相机姿态调整, pitch, ylow, head*/
void BaseApp::UpdatePosByRMouse(Camera* camera, float value){
	acquireInput();

	if(isKeyDown(DIK_A)){
		camera->strafe(-value);
	}
	if(isKeyDown(DIK_D)){
		camera->strafe(value);
	}
	if(isKeyDown(DIK_W)){
		camera->walk(value);
	}
	if(isKeyDown(DIK_S)){
		camera->walk(-value);
	}

	_isRMouseDown = isRMouseDown();

	if(_isRMouseDown){
		float nx = (float)_mouseState.lAxisX;
		float ny = (float)_mouseState.lAxisY;
		float dx = nx - _mouseX;
		float dy = ny - _mouseY;

		camera->rotateY(-2.0f * dx * value);
		camera->pitchRotate(-2.0f * dy * value);

		_mouseX = nx;
		_mouseY = ny;
	}else if(!_isLMouseDown){
		_mouseX = (float)_mouseState.lAxisX;
		_mouseY = (float)_mouseState.lAxisY;
	}
}

void BaseApp::UpdatePosByLMouse(Mesh* mesh, float value){
	acquireInput();

	_isLMouseDown = isLMouseDown();

	if(_isLMouseDown){
		float nx = (float)_mouseState.lAxisX;
		float ny = (float)_mouseState.lAxisY;
		float dx = nx - _mouseX;
		float dy = ny - _mouseY;
		if(abs(dx) > abs(dy)){
			mesh->rotateY(-2.0f * dx * value);
		}else{
			mesh->rotateX(-2.0f * dy * value);
		}

		_mouseX = nx;
		_mouseY = ny;
	}else if(!_isRMouseDown){
		_mouseX = (float)_mouseState.lAxisX;
		_mouseY = (float)_mouseState.lAxisY;
	}
}

string BaseApp::getAppRoot(){
	return _env.getVariable("PLU_ROOT");
}

string BaseApp::getFullPath(char* path){
	return _env.getVariable("PLU_ROOT") + path;
}

wstring BaseApp::getFullPathW(char* path){
	string pstr = _env.getVariable("PLU_ROOT") + path;
	wstring wst;
	str_2_wstr(pstr, wst);
	return wst;
}
