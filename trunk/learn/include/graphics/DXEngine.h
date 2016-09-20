#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <BaseDataStruct.h>
#include <DirectXColors.h>
#include <graphics/DXVisual.h>
#include <graphics/VertexShader.h>
#include <graphics/PixelShader.h>
#include <graphics/DXRenderTexture.h>

class DXEngine{
public:
	DXEngine();
	~DXEngine();

	void InitDevice(HWND const &hwnd, int screenWidth, int screenHeight);
	bool CreateDevice(HWND const &hwnd, int screenWidth, int screenHeight);
	void InitVisual(DXVisual &visual, void* vertices, int vertexNum, int* indices, int indexNum);
	inline void SetDefaultRenderTargetView();
	inline void DrawVisual(DXVisual &visual);
	inline void DrawVisual2RTT(DXVisual &visual);

	//创建rtt相关
	inline void CreateRTT();
	inline void UseRTT();

public:
	inline bool GetReady();
	inline ID3D11Device* GetDevice() const;

	inline void UpdateVSSubResource(DXVisual &visual, int buffIndex, const void* data);
	inline void UpdatePSSubResource(DXVisual &visual, int buffIndex, const void* data);
	inline void Present();
	void ClearBuffers(const FLOAT ColorRGBA[4] = DirectX::Colors::MidnightBlue);
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

	/*默认缓存视图*/
	ID3D11Texture2D* _renderTargetBuffer;
	ID3D11RenderTargetView* _renderTargetView;

	/*默认模板视图*/
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11DepthStencilState* _depthStencilState;

	/*默认光栅化状态*/
	ID3D11RasterizerState* _wireframeRS;
	ID3D11RasterizerState* _solidRS;
	/*默认混合状态*/
	ID3D11BlendState* _blendState;
	ID3D11BlendState* _blendEnableState;
	ID3D11BlendState* _blendDisableState;

	//RTT
	DXRenderTexture _renderTexture;
private:
	/*创建深度缓存*/
	bool CreateDepthStencilView();

	/*创建渲染视图*/
	bool CreateRenderTargetlView();

	/*创建视口*/
	void CreateViewPort();

	/*默认创建开启和关闭的blendstate*/
	void CreateAlphaBlendState();
	void EnableAlphaBlend();
	void DisableAlphaBlend();

	bool CreateDepthState();
	bool CreateRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs);
};


inline ID3D11Device* DXEngine::GetDevice() const{
	return _device;
}

inline bool DXEngine::GetReady(){
	return _ready;
}

inline void DXEngine::SetDefaultRenderTargetView(){
	_context->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
}

inline void DXEngine::UpdateVSSubResource(DXVisual &visual, int buffIndex, const void* data){
	visual.UpdateVSConstBuffer(_context, buffIndex, data);
}

inline void DXEngine::UpdatePSSubResource(DXVisual &visual, int buffIndex, const void* data){
	visual.UpdatePSConstBuffer(_context, buffIndex, data);
}

inline void DXEngine::CreateRTT(){
	_renderTexture.CreateRenderTargetView(_device, _width, _height);
}

inline void DXEngine::UseRTT(){
	_renderTexture.UseRTT(_context, _depthStencilView);
	_renderTexture.ClearRTT(_context, _depthStencilView);
}

inline void DXEngine::Present(){
	_chain->Present(0, 0);
}

inline void DXEngine::DrawVisual(DXVisual &visual){
	visual.Draw(_context);
}

inline void DXEngine::DrawVisual2RTT(DXVisual &visual){
	visual.Draw(_context, _renderTexture.GetRTTResView());
}
