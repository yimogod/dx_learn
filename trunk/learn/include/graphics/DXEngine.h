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
	/*Ĭ�ϲ���״̬*/
	ID3D11SamplerState* _sampleState;

	ID3D11Buffer* _vertexBuff = nullptr;
	ID3D11Buffer* _indexBuff = nullptr;
	ID3D11Buffer* _constBuff = nullptr;
	/*����ͼ��Դ���õ�resource view, ���һ������Ҫ����ͼ, ��ô���ǵ�_resView�ͻ��Ǹ�����*/
	int _resViewNum = 0;
	ID3D11ShaderResourceView* _resView[8];

private:
	/*������Ȼ���*/
	bool createDepthStencilView();

	/*������Ⱦ��ͼ*/
	bool createRenderTargetlView();

	/*�����ӿ�*/
	void createViewPort();

	/*Ĭ�ϴ��������͹رյ�blendstate*/
	void createAlphaBlendState();
	void enableAlphaBlend();
	void disableAlphaBlend();

	bool createSamplerState();
	bool createTexture(const wchar_t* path);


	/*����shaderʹ�õ�texture2d*/
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
