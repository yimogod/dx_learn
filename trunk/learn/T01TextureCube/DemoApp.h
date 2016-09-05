#pragma once
#include <windows.h>
#include <Main.h>
#include <BaseApp.h>

class DemoApp : public BaseApp{
public:
	DemoApp();
	~DemoApp();

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Update();
	virtual void Render();
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
