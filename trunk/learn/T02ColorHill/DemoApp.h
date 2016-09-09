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

	virtual void Render();

protected:
	/* all parm size is base on meter */
	void CreateGrid(float width, float depth, float unitSize, Mesh &mesh);
	float GetVertexHeight(float x, float z);
	Color GetColorFromHeight(float y);
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow){
	MainInfo info;
	info.app = new DemoApp();
	info.icon = nullptr;
	info.title = L"ColorHill";
	info.width = 1200;
	info.height = 960;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
