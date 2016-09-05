#include <d3dcompiler.h>
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
