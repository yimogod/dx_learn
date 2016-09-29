#pragma once
#include <windows.h>
#include <Main.h>
#include <sys/Window.h>

class DemoApp : public Window{
public:
	DemoApp();
	virtual ~DemoApp();

	virtual bool LoadContent();
	virtual void UnloadContent();

	void RenderOneTime();
	void UpdateConstForPhong();
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
	info.title = L"Shadow Map";
	info.width = 1280;
	info.height = 960;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
