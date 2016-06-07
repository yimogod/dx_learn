#pragma once
#include <windows.h>
#include <Main.h>

#include <DirectXMath.h>
#include <dinput.h>
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

protected:
	/* all parm size is base on meter */
	void createGrid(float width, float depth, float unitSize, Mesh &mesh);
	float getVertexHeight(float x, float z);
	Color getColorFromHeight(float y);
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow){
	MainInfo info;
	info.app = new DemoApp();
	info.icon = nullptr;
	info.title = L"TextureCube";
	info.width = 800;
	info.height = 600;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
