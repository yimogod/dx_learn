#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include "DemoApp1.h"
#include "../core/Application.h"
#include "../util/DDSTextureLoader.h"

DemoApp1::DemoApp1(){}

DemoApp1::~DemoApp1(){}

bool DemoApp1::createVertexBuffer(){
	VertexPos vertices[] = {
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
	
		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
	};

	/*设置buff desc*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexPos) * 3;
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
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
		L"E:/learn/dx_learn/trunk/learn/Tutorial01/src/assets/seafloor.dds";
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
	HRESULT hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&_inputDevice, nullptr);
	if(FAILED(hr))return false;
	
	hr = _inputDevice->CreateDevice(GUID_SysMouse, &_mouse, nullptr);
	if(FAILED(hr))return false;


	_inputObjFormat[0] = {&GUID_XAxis, FIELD_OFFSET(MouseState, lAxisX),    // X axis
		DIDFT_AXIS|DIDFT_ANYINSTANCE, 0};
	_inputObjFormat[1] = {&GUID_YAxis, FIELD_OFFSET(MouseState, lAxisY),    // Y axis
		DIDFT_AXIS|DIDFT_ANYINSTANCE, 0};
	_inputObjFormat[2] = {0, FIELD_OFFSET(MouseState, abButtons[0]),        // Button 0
		DIDFT_BUTTON|DIDFT_ANYINSTANCE, 0};
	_inputObjFormat[3] = {0, FIELD_OFFSET(MouseState, abButtons[1]),        // Button 1 (optional)
		DIDFT_BUTTON|DIDFT_ANYINSTANCE, 0};
	_inputObjFormat[4] = {0, FIELD_OFFSET(MouseState, abButtons[2]),        // Button 2 (optional)
		DIDFT_BUTTON|DIDFT_ANYINSTANCE, 0};

	int numMouseObjects = 5;
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

	return true;
}

bool DemoApp1::loadContent(){
	createDXInput();
	createShader();
	createVertexBuffer();
	createTexture();

	UINT stride = sizeof(VertexPos);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_vertexBuff, &stride, &offset);

	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	return true;
}

void DemoApp1::unloadContent(){
	if(_mouse){
		_mouse->Unacquire();
		_mouse->Release();
	}
	if(_inputDevice)_inputDevice->Release();
	if(_vs)_vs->Release();
	if(_ps)_ps->Release();
	if(_vertexLayout)_vertexLayout->Release();
	if(_vertexBuff)_vertexBuff->Release();

	_mouse = nullptr;
	_inputDevice = nullptr;
	_vs = nullptr;
	_ps = nullptr;
	_vertexLayout = nullptr;
	_vertexBuff = nullptr;
}

void DemoApp1::update(){}

void DemoApp1::render(){
	if(_context == NULL)return;

	_context->ClearRenderTargetView(_backBuffTarget, Colors::MidnightBlue);

	// Render a triangle
	_context->VSSetShader(_vs, nullptr, 0);
	_context->PSSetShader(_ps, nullptr, 0);
	_context->PSSetShaderResources(0, 1, &_resView);
	_context->PSSetSamplers(0, 1, &_sampleState);
	_context->Draw(6, 0);

	_chain->Present(0, 0);
}