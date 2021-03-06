#include <d3dcompiler.h>
#include <util/CUtil.h>
#include <sys/BaseApp.h>

BaseApp::BaseApp(){}

BaseApp::~BaseApp(){}

bool BaseApp::Init(HINSTANCE const &ins, HWND const &hwnd){
	_ins = ins;
	_hwnd = hwnd;

	RECT rect;
	GetClientRect(_hwnd, &rect);
	_width = rect.right - rect.left;
	_height = rect.bottom - rect.top;

	return true;
}

void BaseApp::OnMove(int screenX, int screenY){
	_mouseX = screenX;
	_mouseY = screenY;
}

void BaseApp::OnKeyDown(int key, int screenX, int screenY){
	_pressedKey = key;
	_mouseX = screenX;
	_mouseY = screenY;
}

void BaseApp::OnKeyUp(int key, int screenX, int screenY){
	_pressedKey = -1;
	_mouseX = screenX;
	_mouseY = screenY;
}

void BaseApp::OnMouseDown(int screenX, int screenY){
	_isLMouseDown = true;
	_mouseX = screenX;
	_mouseY = screenY;
}

void BaseApp::OnMouseUp(int screenX, int screenY){
	_isLMouseDown = false;
	_mouseX = screenX;
	_mouseY = screenY;
}

void BaseApp::OnRMouseDown(int screenX, int screenY){
	_isRMouseDown = true;
	_mouseX = screenX;
	_mouseY = screenY;
}

void BaseApp::OnRMouseUp(int screenX, int screenY){
	_isRMouseDown = false;
	_mouseX = screenX;
	_mouseY = screenY;
}