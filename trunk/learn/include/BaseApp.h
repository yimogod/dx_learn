#pragma once
#include <d3d11_1.h>
#include <Matrix4x4.h>

struct ConstantBuffer{
	Matrix4x4 mWorld;
	Matrix4x4 mView;
	Matrix4x4 mProjection;
};

struct MouseState{
	long lAxisX;
	long lAxisY;
	char abButtons[3];
	char bPadding;       // Structure must be DWORD multiple in size.
};

class BaseApp{
public:
	BaseApp();
	virtual ~BaseApp();

	bool init(HINSTANCE ins, HWND hwnd);
	void destroy();

	virtual bool loadContent();
	virtual void unloadContent();

	virtual void render();
	virtual void update();

protected:
	HINSTANCE _ins;
	HWND _hwnd;

	float _width, _height;

	D3D_DRIVER_TYPE _driverType;
	D3D_FEATURE_LEVEL _featureLevel;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;
	IDXGISwapChain* _chain;
	ID3D11RenderTargetView* _backBuffTarget;

	HRESULT compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint,
		LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};