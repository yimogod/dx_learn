#pragma once
#include <windows.h>
#include <Main.h>
#include "resource.h"

#include <DirectXMath.h>
#include <dinput.h>
#include <BaseApp.h>

using namespace DirectX;

struct ScrollBuffer{
	float scroll;
	float pad0, pad1, pad2;
};

class DemoApp : public BaseApp{
public:
	DemoApp();
	virtual ~DemoApp();

	bool loadContent();
	void unloadContent();

	void update();
	void render();

private:
	ID3D11Buffer* _scrollBuff = nullptr;
	ScrollBuffer _scroll;
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