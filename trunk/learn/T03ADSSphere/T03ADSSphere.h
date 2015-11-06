#pragma once
#include <windows.h>
#include <Main.h>
#include "resource.h"

#include <DirectXMath.h>
#include <dinput.h>
#include <Scene.h>
#include <Matrix4x4.h>
#include <BaseApp.h>

using namespace DirectX;

class T03ADSSphere : public BaseApp{
public:
	T03ADSSphere();
	virtual ~T03ADSSphere();

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
	info.app = new T03ADSSphere();
	info.icon = (LPCWSTR)IDI_TUTORIAL1;
	info.title = L"TextureCube";
	info.width = 800;
	info.height = 600;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
