#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <graphics/Vertex.h>
#include <graphics/VertexShader.h>
#include <graphics/PixelShader.h>

class DXEngine{
public:
	DXEngine();
	~DXEngine();

	void InitDevice(HWND const &hwnd, int screenWidth, int screenHeight);
	bool CreateDevice(HWND const &hwnd, int screenWidth, int screenHeight);

	//编译并创建shader
	bool CreateShader(VertexShader &vs, PixelShader &ps, InputLayout &layout);
	//创建/绑定各种buff
	bool CreateVertexBuffer(Vertex *vertices, int vertNum, int vertSize);
	bool CreateVertexBuffer(Vertex *vertices, int byteWidth, ID3D11Buffer** vertexBuff);
	void BindVertexBuff();
	void BindVertexBuff(ID3D11Buffer* vertexBuff);

	bool CreateIndexBuffer(unsigned short* indexList, int indexNum);
	void BindIndexBuff();
	
	bool CreateConstBuffer(ID3D11Buffer** constBuff, int byteWidth);
	bool CreateConstBuffer(int byteWidth);

	bool CreateTexture(const wchar_t* path);

public:
	inline bool GetReady();
	inline ID3D11Device* GetDevice() const;
	inline ID3D11DeviceContext* GetContext() const;
	inline ID3D11Buffer* GetConstBuff() const;

	void ClearBuffers(const FLOAT ColorRGBA[4] = DirectX::Colors::MidnightBlue);
	void Present();
	//device或者context的一些代理方法
	inline void VSSetShader();
	inline void VSSetConstantBuffers(UINT StartSlot, UINT NumBuffer);
	inline void PSSetShader();
	inline void PSSetShaderResources(UINT StartSlot);
	inline void PSSetSamplers(UINT StartSlot, UINT NumSampler);

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
	ID3D11ShaderResourceView* _renderTargetResView;

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

	/*默认采样状态*/
	ID3D11SamplerState* _sampleState;

	ID3D11Buffer* _vertexBuff = nullptr;
	ID3D11Buffer* _indexBuff = nullptr;
	ID3D11Buffer* _constBuff = nullptr;
	/*对贴图资源引用的resource view, 如果一个面需要多张图, 那么我们的_resView就会是个数组*/
	int _resViewNum = 0;
	ID3D11ShaderResourceView* _resView[8];
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

	bool CreateSamplerState();

	/*创建shader使用的texture2d*/
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

inline ID3D11Buffer* DXEngine::GetConstBuff() const{
	return _constBuff;
}

inline bool DXEngine::GetReady(){
	return _ready;
}

inline void DXEngine::VSSetShader(){
	_context->VSSetShader(_vs, nullptr, 0);
}

inline void DXEngine::VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers){
	_context->VSSetConstantBuffers(StartSlot, NumBuffers, &_constBuff);
}

inline void DXEngine::PSSetShader(){
	_context->PSSetShader(_ps, nullptr, 0);
}

inline void DXEngine::PSSetShaderResources(UINT StartSlot){
	_context->PSSetShaderResources(StartSlot, _resViewNum, _resView);
}

inline void DXEngine::PSSetSamplers(UINT StartSlot, UINT NumSamplers){
	_context->PSSetSamplers(StartSlot, NumSamplers, &_sampleState);
}