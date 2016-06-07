#pragma once
#include <windows.h>
#include <Main.h>

#include <dinput.h>
#include <DirectXMath.h>
#include <graphics/Scene.h>
#include <math/algebra/Matrix4x4.h>
#include <BaseApp.h>

using namespace DirectX;

struct FogBuffer{
	float fogMinDis;
	float fogMaxDis;
	float padding1, padding2;//´Õ³É4µÄ±¶Êý
};

class DemoApp : public BaseApp{
public:
	DemoApp();
	virtual ~DemoApp();

	bool loadContent();
	void unloadContent();

	void update();
	void render();

protected:
	ID3D11Buffer* _fogBuff = nullptr;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow){
	MainInfo info;
	info.app = new DemoApp();
	info.icon = nullptr;
	info.title = L"TextureCube";
	info.width = 1200;
	info.height = 960;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
