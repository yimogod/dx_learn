#pragma once
#include <windows.h>
#include <string>
#include <d3d11_1.h>
#include <dinput.h>
#include <math/algebra/Matrix4x4.h>
#include <graphics/Vertex.h>
#include <sys/SysEnv.h>
#include <graphics/Scene.h>

namespace plu{
	using namespace std;

	struct ConstantBuffer{
		Matrix4x4 model;
		Matrix4x4 view;
		Matrix4x4 perspective;
	};

	struct MouseState{
		long lAxisX;
		long lAxisY;
		char abButtons[3];
		char bPadding;       // Structure must be DWORD multiple in size.
	};

	struct CreateShaderInfo{
		WCHAR* fileName;
		LPCSTR entryPoint;
		LPCSTR shaderModel;
	};

	class DX11Engine{
	public:
		DX11Engine(HWND hwnd);
		~DX11Engine();

	protected:
		HINSTANCE _ins;
		HWND _hwnd;

		int _width, _height;

		D3D_DRIVER_TYPE _driverType;
		D3D_FEATURE_LEVEL _featureLevel;

		ID3D11Device* _device;
		ID3D11DeviceContext* _context;
		IDXGISwapChain* _chain;

		/*������ͼ*/
		ID3D11Texture2D* _renderTargetBuffer;
		ID3D11RenderTargetView* _renderTargetView;

		/*ģ����ͼ*/
		ID3D11Texture2D* _depthStencilBuffer;
		ID3D11DepthStencilView* _depthStencilView;

		/*��Ⱦ�ܵ��ĸ���״̬*/
		/*��դ��״̬*/
		ID3D11RasterizerState* _wireframeRS;
		ID3D11RasterizerState* _solidRS;
		/*���״̬*/
		ID3D11BlendState* _blendEnableState;
		ID3D11BlendState* _blendDisableState;
		/*����״̬*/
		ID3D11SamplerState* _sampleState;
		/*��Ȼ���״̬*/
		ID3D11DepthStencilState* _depthStencilState;


		HRESULT compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint,
			LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

		ID3D11VertexShader* _vs = nullptr;
		ID3D11PixelShader*  _ps = nullptr;
		ID3D11InputLayout*  _vertexLayout = nullptr;
		ID3D11Buffer* _vertexBuff = nullptr;
		ID3D11Buffer* _indexBuff = nullptr;
		ID3D11Buffer* _constBuff = nullptr;
		/*����ͼ��Դ���õ�resource view, ���һ������Ҫ����ͼ, ��ô���ǵ�_resView�ͻ��Ǹ�����*/
		int _resViewNum = 0;
		ID3D11ShaderResourceView* _resView[8];

	protected:
		void init();
		void destroy();

		void initDevice();

		bool createDevice();

		/*������Ȼ���*/
		bool createDepthStencilView();

		/*������Ⱦ��ͼ*/
		bool createRenderTargetlView();

		/*�����ӿ�*/
		void createViewPort();

		bool createShader(CreateShaderInfo vs, CreateShaderInfo ps, D3D11_INPUT_ELEMENT_DESC layout[], int numElements);
		bool createVertexBuffer(Vertex *vertices, int vertNum, int vertSize);
		bool createVertexBuffer(Vertex *vertices, int byteWidth, ID3D11Buffer** vertexBuff);
		void bindVertexBuff();
		void bindVertexBuff(ID3D11Buffer* vertexBuff);
		bool createIndexBuffer(unsigned short* indexList, int indexNum);
		void bindIndexBuff();
		bool createConstBuffer(ID3D11Buffer** constBuff, int byteWidth);

		bool createDepthState();
		bool createRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs);

		/*Ĭ�ϴ��������͹رյ�blendstate*/
		void createAlphaBlendState();
		void enableAlphaBlend();
		void disableAlphaBlend();

		bool createSamplerState();
		bool createTexture(const wchar_t* path);

	};
}


