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

	_dxEngine.InitDevice(hwnd, _width, _height);

	return LoadContent();
}

void BaseApp::Destroy(){
	UnloadContent();
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

string const BaseApp::GetAppRoot(){
	return _env.getVariable("PLU_ROOT");
}

string BaseApp::GetFullPath(char* path){
	return _env.getVariable("PLU_ROOT") + path;
}

wstring BaseApp::GetFullPathW(char* path){
	string pstr = _env.getVariable("PLU_ROOT") + path;
	wstring wst;
	str_2_wstr(pstr, wst);
	return wst;
}
