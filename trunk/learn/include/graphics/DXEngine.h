#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <BaseDataStruct.h>
#include <DirectXColors.h>
#include <graphics/DXVisual.h>
#include <graphics/VertexShader.h>
#include <graphics/PixelShader.h>
#include <graphics/DXRenderTexture.h>
#include <graphics/DepthState.h>

class DXEngine{
public:
	DXEngine();
	~DXEngine();

	void InitDevice(HWND const &hwnd, int screenWidth, int screenHeight);
	bool CreateDevice(HWND const &hwnd, int screenWidth, int screenHeight);
	void InitVisual(DXVisual &visual, void* vertices, int vertexNum, int* indices, int indexNum);
	inline void SetDefaultRenderTargetView();
	inline void DrawVisual(DXVisual &visual);
	inline void DrawVisualByRTT(DXVisual &visual, int index = 0);

	//创建rtt相关
	void CreateRTT(int width = 0, int height = 0);
	inline void UseRTT(int index = 0);

public:
	inline bool GetReady();
	inline ID3D11Device* GetDevice() const;

	inline void UpdateVSSubResource(DXVisual &visual, int buffIndex, const void* data);
	inline void UpdatePSSubResource(DXVisual &visual, int buffIndex, const void* data);
	inline void Present();
	void ClearBuffers(const FLOAT ColorRGBA[4] = DirectX::Colors::MidnightBlue);

	void EnableAlphaBlend();
	void DisableAlphaBlend();
	inline void TurnOnZBuffer();
	inline void TurnOffZBuffer();
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

	DepthState _defaultDepthState;
	DepthState _disableDepthState;

	/*默认光栅化状态*/
	ID3D11RasterizerState* _wireframeRS;
	ID3D11RasterizerState* _solidRS;
	/*默认混合状态*/
	ID3D11BlendState* _blendState;
	ID3D11BlendState* _blendEnableState;
	ID3D11BlendState* _blendDisableState;

	//RTT
	DXRenderTexture _renderTexture[4];
	int _renderTextureNum = 0;
private:
	/*创建深度缓存*/
	bool CreateDepthStencilView();

	/*创建渲染视图*/
	bool CreateRenderTargetlView();

	/*创建视口*/
	void CreateViewPort();

	/*默认创建开启和关闭的blendstate*/
	void CreateAlphaBlendState();

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
	_context->OMSetRenderTargets(1, &_renderTargetView, _defaultDepthState.GetDepthStencilView());
}

inline void DXEngine::UpdateVSSubResource(DXVisual &visual, int buffIndex, const void* data){
	visual.UpdateVSConstBuffer(_context, buffIndex, data);
}

inline void DXEngine::UpdatePSSubResource(DXVisual &visual, int buffIndex, const void* data){
	visual.UpdatePSConstBuffer(_context, buffIndex, data);
}

inline void DXEngine::TurnOnZBuffer(){
	_defaultDepthState.SetDepthState(_context);
}

inline void DXEngine::TurnOffZBuffer(){
	_disableDepthState.SetDepthState(_context);
}

inline void DXEngine::UseRTT(int index){
	_renderTexture[index].UseRTT(_context);
	_renderTexture[index].ClearRTT(_context);
}

inline void DXEngine::Present(){
	_chain->Present(0, 0);
}

inline void DXEngine::DrawVisual(DXVisual &visual){
	visual.Draw(_context);
}

inline void DXEngine::DrawVisualByRTT(DXVisual &visual, int index){
	visual.Draw(_context, _renderTexture[index].GetRTTResView());
}
