#pragma once
#include <windows.h>
#include <Main.h>
#include <sys/Window.h>

struct FogBuffer{
	float fogMinDis;
	float fogMaxDis;
	float padding1, padding2;//´Õ³É4µÄ±¶Êý
};

class DemoApp : public Window{
public:
	DemoApp();
	virtual ~DemoApp();

	virtual bool LoadContent();
	virtual void PreAddOtherConstBuffer(DXVisual &visual);
	virtual void UnloadContent();

	virtual void Update();
	virtual void Render();

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
	info.title = L"Simple Cube";
	info.width = 1200;
	info.height = 960;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
