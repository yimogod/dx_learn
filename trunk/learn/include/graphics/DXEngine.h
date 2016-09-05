#pragma once
#include <graphics/DXInclude.h>

class DXEngine
{
public:
	DXEngine();
	~DXEngine();

	void InitDevice(HWND hwnd, int screenWidth, int screenHeight);

	bool CreateDevice(HWND hwnd, int screenWidth, int screenHeight);

private:
	HINSTANCE _ins;
	HWND _hwnd;

	int _width, _height;

	D3D_DRIVER_TYPE _driverType;
	D3D_FEATURE_LEVEL _featureLevel;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;
	IDXGISwapChain* _chain;

	//layout
	ID3D11InputLayout* _vertexLayout;

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

	ID3D11VertexShader* _vs;
	ID3D11PixelShader* _ps;

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
	bool CreateTexture(const wchar_t* path);


	/*创建shader使用的texture2d*/
	bool CreateRenderTargetViewByShaderRes();

	bool CreateShader(Shader &vs, Shader &ps, D3D11_INPUT_ELEMENT_DESC layout[], int numElements);
	bool CreateVertexBuffer(Vertex *vertices, int vertNum, int vertSize);
	bool CreateVertexBuffer(Vertex *vertices, int byteWidth, ID3D11Buffer** vertexBuff);
	void BindVertexBuff();
	void BindVertexBuff(ID3D11Buffer* vertexBuff);
	bool CreateIndexBuffer(unsigned short* indexList, int indexNum);
	void BindIndexBuff();
	bool CreateConstBuffer(ID3D11Buffer** constBuff, int byteWidth);

	bool CreateDepthState();
	bool CreateRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs);
};
