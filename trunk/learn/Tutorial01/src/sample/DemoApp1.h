#pragma once
#include <DirectXMath.h>
#include <dinput.h>
#include "BaseApp.h"

using namespace DirectX;

struct VertexPos{
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
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
	ID3D11ShaderResourceView* _resView;
	ID3D11SamplerState* _sampleState;

	/*dx input*/
	IDirectInput8* _inputDevice;
	IDirectInputDevice8* _mouse;
	IDirectInputDevice8* _keyborad;
	DIDATAFORMAT _mouseDataFormat;
	DIOBJECTDATAFORMAT _inputObjFormat[5];

	bool createShader();
	bool createVertexBuffer();
	bool createTexture();
	bool createDXInput();

protected:
	char _keyboardBuff[256];
	MouseState _mouseState;
	void onKeyDown(char keycode);
};