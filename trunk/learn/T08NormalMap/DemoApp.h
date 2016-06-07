#pragma once
#include <windows.h>
#include <Main.h>

#include <DirectXMath.h>
#include <dinput.h>
#include <BaseApp.h>
#include <BaseDataStruct.h>

/*·½Ïò¹â*/
struct DirectionLight{
	Float4A ambientColor;
	Float4A diffuseColor;
	Float4A specularColor;
	Float4 direction;
};

struct PhongBuffer{
	Float4 eyeWorldPos;
	DirectionLight directionLight;
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
	ID3D11Buffer* _phongBuff = nullptr;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow){
	MainInfo info;
	info.app = new DemoApp();
	info.icon = nullptr;
	info.title = L"Normal map";
	info.width = 800;
	info.height = 600;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
