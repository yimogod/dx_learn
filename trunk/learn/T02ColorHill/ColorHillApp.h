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

class ColorHillApp : public BaseApp{
public:
	ColorHillApp();
	virtual ~ColorHillApp();

	bool loadContent();
	void unloadContent();

	void update();
	void render();

protected:
	ID3D11VertexShader* _vs = nullptr;
	ID3D11PixelShader*  _ps = nullptr;
	ID3D11InputLayout*  _vertexLayout = nullptr;
	ID3D11Buffer* _vertexBuff = nullptr;
	ID3D11Buffer* _indexBuff = nullptr;
	ID3D11Buffer* _constBuff = nullptr;
	ID3D11ShaderResourceView* _resView;
	ID3D11SamplerState* _sampleState;

	/*dx input*/
	IDirectInput8* _inputDevice;
	IDirectInputDevice8* _mouse;
	IDirectInputDevice8* _keyborad;
	DIDATAFORMAT _mouseDataFormat;
	DIOBJECTDATAFORMAT _inputObjFormat[5];

	/* mvp */
	Matrix4x4 world_to_camera;
	Matrix4x4 camera_to_project;
	Matrix4x4 project_to_screen;

	bool createShader();
	bool createVertexBuffer();
	bool createTexture();
	bool createDXInput();

protected:
	char _keyboardBuff[256];
	MouseState _mouseState;
	bool isKeyDown(char keycode);
	void onKeyDown(char keycode);

	Scene _scene;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow){
	MainInfo info;
	info.app = new ColorHillApp();
	info.icon = (LPCWSTR)IDI_TUTORIAL1;
	info.title = L"ColorHill";
	info.width = 800;
	info.height = 600;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
