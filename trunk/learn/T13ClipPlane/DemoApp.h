#pragma once
#include <windows.h>
#include <Main.h>
#include <sys/Window.h>

struct ScrollBuffer{
	float scroll;
	float pad0, pad1, pad2;
};

class DemoApp : public Window{
public:
	DemoApp();
	virtual ~DemoApp();

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Update();
	virtual void Render();

private:
	ScrollBuffer _scroll;
	Float4 _clipBuffer;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow){
	MainInfo info;
	info.app = new DemoApp();
	info.icon = nullptr;
	info.title = L"Clip Plane";
	info.width = 1200;
	info.height = 960;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
