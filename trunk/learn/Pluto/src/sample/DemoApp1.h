#pragma once
#include <DirectXMath.h>
#include <dinput.h>
#include "../core/Scene.h"
#include "../core/Matrix4x4.h"
#include "../BaseApp.h"

using namespace DirectX;

struct ConstantBuffer{
	Matrix4x4 mWorld;
	Matrix4x4 mView;
	Matrix4x4 mProjection;
};


struct MouseState{
	LONG lAxisX;
	LONG lAxisY;
	BYTE abButtons[3];
	BYTE bPadding;       // Structure must be DWORD multiple in size.
};

class DemoApp1 : public BaseApp{
public:
	DemoApp1();
	virtual ~DemoApp1();

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