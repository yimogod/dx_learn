#include <d3dcompiler.h>
#include <util/CUtil.h>
#include "BaseApp.h"

BaseApp::BaseApp(){}

BaseApp::~BaseApp(){
}

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
