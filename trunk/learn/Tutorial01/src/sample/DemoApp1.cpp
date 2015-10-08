#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include "DemoApp1.h"
#include "../util/FBXReader.h"
#include "../util/DDSTextureLoader.h"


#define DIRECTINPUT_VERSION 0x0800

DemoApp1::DemoApp1(){}

DemoApp1::~DemoApp1(){}

bool DemoApp1::createVertexBuffer(){
	/*char* sInputFile = "assets/simple_scene.fbx";
	FBXReader reader;
	reader.read(sInputFile, &_scene);
	_scene.buildMesh();
	_scene.renderType = Scene::RENDER_TYPE_FRAME;

	_scene.camera = new Camera(0, 0, -10.0f, 0, 0, 0);
	_scene.camera->setProperty(1.0f, 45.0f, 1.0f, 100.0f, 800.0f, 600.0f);

	_scene.lightList[0] = new Light();
	_scene.lightList[0]->type = Light::TYPE_AMBIENT;
	_scene.lightList[0]->ambientColor = Color(0.0f, 1.0f, 1.0f);
	_scene.lightNum = 1;*/

	XMFLOAT3 vertices[] =
	{
		{0.0f, 0.5f, 0.5f},
		{0.5f, -0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f},
	};

	/*XMFLOAT3 vertices[] = {
		{-1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{-1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, 1.0f},
		{-1.0f, -1.0f, 1.0f},
	};*/

	WORD indices[] = {
		2,1,0,
	};

	/*WORD indices[] = {
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};*/

	/*设置buff desc*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(XMFLOAT3) * 3;//数据总长度
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
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_vertexBuff, &stride, &offset);


	/*设置index buff*/
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 3;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	/*创建index buff*/
	InitData.pSysMem = indices;
	hr = _device->CreateBuffer(&bd, &InitData, &_indexBuff);
	if(FAILED(hr))return false;

	/*设置当前vertex buff*/
	_context->IASetIndexBuffer(_indexBuff, DXGI_FORMAT_R16_UINT, 0);

	/*设置三角形排列方式*/
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	/*创建constant buff, 类似于uniform变量*/
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _device->CreateBuffer(&bd, nullptr, &_constBuff);
	if(FAILED(hr))return false;

	return true;
}

bool DemoApp1::createShader(){
	/*编译shader*/
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = compileShaderFromFile(L"shader/Triangle.fx", "VS", "vs_4_0", &pVSBlob);
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

	/*创建 layout*/
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = ARRAYSIZE(layout);
	
	hr = _device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
								pVSBlob->GetBufferSize(), &_vertexLayout);
	pVSBlob->Release();
	if(FAILED(hr))return false;
	
	/*设置 layout*/
	_context->IASetInputLayout(_vertexLayout);

	/*编译shader*/
	ID3DBlob* pPSBlob = nullptr;
	hr = compileShaderFromFile(L"shader/Triangle.fx", "PS", "ps_4_0", &pPSBlob);
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

bool DemoApp1::createTexture(){
	const wchar_t* path =
		L"E:/learn/dx_learn/trunk/learn/Tutorial01/assets/seafloor.dds";
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
}

bool DemoApp1::createDXInput(){
	ZeroMemory(_keyboardBuff, sizeof(_keyboardBuff));

	HRESULT hr = DirectInput8Create(_ins, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&_inputDevice, nullptr);
	if(FAILED(hr))return false;
	
	/*创建鼠标*/
	hr = _inputDevice->CreateDevice(GUID_SysMouse, &_mouse, nullptr);
	if(FAILED(hr))return false;


	_inputObjFormat[0] = {&GUID_XAxis, FIELD_OFFSET(MouseState, lAxisX),    // X axis
		DIDFT_AXIS|DIDFT_ANYINSTANCE, 0};
	_inputObjFormat[1] = {&GUID_YAxis, FIELD_OFFSET(MouseState, lAxisY),    // Y axis
		DIDFT_AXIS|DIDFT_ANYINSTANCE, 0};
	_inputObjFormat[2] = {0, (DWORD)FIELD_OFFSET(MouseState, abButtons[0]),        // Button 0
		DIDFT_BUTTON|DIDFT_ANYINSTANCE, 0};
	_inputObjFormat[3] = {0, (DWORD)FIELD_OFFSET(MouseState, abButtons[1]),        // Button 1 (optional)
		DIDFT_BUTTON|DIDFT_ANYINSTANCE, 0};
	_inputObjFormat[4] = {0, (DWORD)FIELD_OFFSET(MouseState, abButtons[2]),        // Button 2 (optional)
		DIDFT_BUTTON|DIDFT_ANYINSTANCE, 0};

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

	hr = _mouse->SetCooperativeLevel(_hwnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	if(FAILED(hr))return false;

	hr = _mouse->Acquire();
	if(FAILED(hr))return false;
	
	/*创建键盘*/
	hr = _inputDevice->CreateDevice(GUID_SysKeyboard, &_keyborad, nullptr);
	if(FAILED(hr))return false;

	hr = _keyborad->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))return false;

	hr = _keyborad->SetCooperativeLevel(_hwnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	if(FAILED(hr))return false;

	return true;
}

bool DemoApp1::loadContent(){
	createDXInput();
	createShader();
	createVertexBuffer();
	createTexture();
	
	return true;
}

void DemoApp1::unloadContent(){
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

void DemoApp1::update(){
	/*按下键盘*/
	HRESULT hr = _keyborad->Acquire();
	if(SUCCEEDED(hr)){
		hr = _keyborad->GetDeviceState(sizeof(_keyboardBuff), (LPVOID)&_keyboardBuff);
		if(SUCCEEDED(hr)){
			char a = _keyboardBuff[DIK_A];
			if(a&0x80)onKeyDown(a);
		}
	}

	/*按下鼠标*/
	hr = _mouse->Acquire();
	if(SUCCEEDED(hr)){
		hr = _mouse->GetDeviceState(sizeof(MouseState), &_mouseState);
		if(SUCCEEDED(hr)){
			//i want a simple ui
		}
	}
}

void DemoApp1::onKeyDown(char keycode){
	
}

void DemoApp1::render(){
	if(_context == NULL)return;

	_context->ClearRenderTargetView(_backBuffTarget, Colors::MidnightBlue);

	// Render a triangle
	_context->VSSetShader(_vs, nullptr, 0);
	_context->PSSetShader(_ps, nullptr, 0);
	//_context->PSSetShaderResources(0, 1, &_resView);
	//_context->PSSetSamplers(0, 1, &_sampleState);
	_context->DrawIndexed(3, 0, 0);
	//_context->Draw(3, 0);

	_chain->Present(0, 0);
}