#pragma once
#include <memory>
#include <mutex>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <BaseDataStruct.h>
#include <DirectXColors.h>
#include <graphics/DXVisual.h>
#include <graphics/DXVertexShader.h>
#include <graphics/DXPixelShader.h>
#include <graphics/DXRenderTexture.h>
#include <graphics/DXDepthState.h>

//��߱����namespce, ���������ظ�����Ĵ���
namespace{
	std::mutex singleton_mutex;
}

class DXEngine{
public:
	DXEngine();
	~DXEngine();

	static DXEngine& Instance();
	static void Destroy();

	void InitDevice(HWND const &hwnd, int screenWidth, int screenHeight);
	bool CreateDevice(HWND const &hwnd, int screenWidth, int screenHeight);
	
	inline void SetDefaultRenderTargetView();
	inline void DrawVisual(DXVisual &visual);
	inline void DrawVisualByRTT(DXVisual &visual, int index = 0);

	//����rtt���
	void CreateRTT(int width = 0, int height = 0);
	inline void UseRTT(int index = 0);

public:
	inline bool GetReady();
	inline ID3D11Device* GetDevice() const;
	inline ID3D11DeviceContext* GetContext() const;

	inline void UpdatePSSubResource(DXVisual &visual, int buffIndex, const void* data);
	inline void Present();
	void ClearBuffers(const FLOAT ColorRGBA[4] = DirectX::Colors::MidnightBlue);

	void EnableAlphaBlend();
	void DisableAlphaBlend();
	inline void TurnOnZBuffer();
	inline void TurnOffZBuffer();
private:
	static std::unique_ptr<DXEngine> _instance;

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

	//��Ȼ���
	DepthState _defaultDepthState;
	DepthState _disableDepthState;

	/*Ĭ�Ϲ�դ��״̬*/
	ID3D11RasterizerState* _wireframeRS;
	ID3D11RasterizerState* _solidRS;
	/*Ĭ�ϻ��״̬*/
	ID3D11BlendState* _blendState;
	ID3D11BlendState* _blendEnableState;
	ID3D11BlendState* _blendDisableState;

	//RTT
	RenderTexture _renderTexture[4];
	int _renderTextureNum = 0;
private:
	/*������Ȼ���*/
	bool CreateDepthStencilView();

	/*������Ⱦ��ͼ*/
	bool CreateRenderTargetlView();

	/*�����ӿ�*/
	void CreateViewPort();

	/*Ĭ�ϴ��������͹رյ�blendstate*/
	void CreateAlphaBlendState();

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

inline void DXEngine::SetDefaultRenderTargetView(){
	_context->OMSetRenderTargets(1, &_renderTargetView, _defaultDepthState.GetDepthStencilView());
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
