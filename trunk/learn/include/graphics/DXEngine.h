#pragma once
#include <d3d11_1.h>

class DXEngine
{
public:
	DXEngine();
	~DXEngine();

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
	bool createDepthStencilView();

	/*创建渲染视图*/
	bool createRenderTargetlView();

	/*创建视口*/
	void createViewPort();

	/*默认创建开启和关闭的blendstate*/
	void createAlphaBlendState();
	void enableAlphaBlend();
	void disableAlphaBlend();

	bool createSamplerState();
	bool createTexture(const wchar_t* path);


	/*创建shader使用的texture2d*/
	bool createRenderTargetViewByShaderRes();

	bool createShader(Shader vs, Shader ps, D3D11_INPUT_ELEMENT_DESC layout[], int numElements);
	bool createVertexBuffer(Vertex *vertices, int vertNum, int vertSize);
	bool createVertexBuffer(Vertex *vertices, int byteWidth, ID3D11Buffer** vertexBuff);
	void bindVertexBuff();
	void bindVertexBuff(ID3D11Buffer* vertexBuff);
	bool createIndexBuffer(unsigned short* indexList, int indexNum);
	void bindIndexBuff();
	bool createConstBuffer(ID3D11Buffer** constBuff, int byteWidth);

	bool createDepthState();
	bool createRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs);
};
