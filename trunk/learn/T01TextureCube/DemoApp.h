#pragma once
#include <windows.h>
#include <Main.h>
#include "resource.h"

#include <dinput.h>
#include <DirectXMath.h>
#include <graphics/Scene.h>
#include <math/algebra/Matrix4x4.h>
#include <BaseApp.h>

using namespace DirectX;

class DemoApp : public BaseApp{
public:
	DemoApp();
	virtual ~DemoApp();

	bool loadContent();
	void unloadContent();

	void update();
	void render();
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow){

	MainInfo info;
	info.app = new DemoApp();
	info.icon = (LPCWSTR)IDI_TUTORIAL1;
	info.title = L"TextureCube";
	info.width = 1200;
	info.height = 960;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
