#include <d3dcompiler.h>
#include <dinput.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <util/DDSTextureLoader.h>
#include <util/CUtil.h>
#include <graphics/Shader.h>
#include "BaseApp.h"

using namespace DirectX;

BaseApp::BaseApp(){}

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
