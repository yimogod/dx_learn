#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <graphics/DXVisual.h>
#include <graphics/Vertex.h>
#include <graphics/VertexShader.h>
#include <graphics/PixelShader.h>

class DXEngine{
public:
	DXEngine();
	~DXEngine();

	void InitDevice(HWND const &hwnd, int screenWidth, int screenHeight);
	bool CreateDevice(HWND const &hwnd, int screenWidth, int screenHeight);
	void InitVisual(DXVisual &visual, void* vertices, int vertexNum, int* indices, int indexNum);
	bool CreateTexture(const wchar_t* path);

	void DrawVisual(DXVisual &visual);

public:
	inline bool GetReady();
	inline ID3D11Device* GetDevice() const;
	inline ID3D11DeviceContext* GetContext() const;

	void ClearBuffers(const FLOAT ColorRGBA[4] = DirectX::Colors::MidnightBlue);
	void Present();
private:
	HINSTANCE _ins;
	HWND _hwnd;
	bool _ready = false;

	int _width, _height;

	D3D_DRIVER_TYPE _driverType;
	D3D_FEATURE_LEVEL _featureLevel;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;
	IDXGISwapChain* _chain;

	/*Ĭ�ϻ�����ͼ*/
	ID3D11Texture2D* _renderTargetBuffer;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11ShaderResourceView* _renderTargetResView;

	/*Ĭ��ģ����ͼ*/
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11DepthStencilState* _depthStencilState;

	/*Ĭ�Ϲ�դ��״̬*/
	ID3D11RasterizerState* _wireframeRS;
	ID3D11RasterizerState* _solidRS;
	/*Ĭ�ϻ��״̬*/
	ID3D11BlendState* _blendState;
	ID3D11BlendState* _blendEnableState;
	ID3D11BlendState* _blendDisableState;
private:
	/*������Ȼ���*/
	bool CreateDepthStencilView();

	/*������Ⱦ��ͼ*/
	bool CreateRenderTargetlView();

	/*�����ӿ�*/
	void CreateViewPort();

	/*Ĭ�ϴ��������͹رյ�blendstate*/
	void CreateAlphaBlendState();
	void EnableAlphaBlend();
	void DisableAlphaBlend();

	/*����shaderʹ�õ�texture2d*/
	bool CreateRenderTargetViewByShaderRes();

	bool CreateDepthState();
	bool CreateRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs);
};


inline ID3D11Device* DXEngine::GetDevice() const{
	return _device;
}

inline ID3D11DeviceContext* DXEngine::GetContext() const{
	return _context;
}

inline bool DXEngine::GetReady(){
	return _ready;
}
