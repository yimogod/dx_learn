#pragma once
#include <memory>
#include <string>
#include <windows.h>
#include <d3d11_1.h>
#include <dinput.h>
#include <math/algebra/Matrix4x4.h>
#include <graphics/Vertex.h>
#include <sys/SysEnv.h>
#include <graphics/Scene.h>

namespace plu{
	using namespace std;
	class BlendState;
	class RasterizerState;
	class DepthStencilState;


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

		/*获取成员变量的get方法*/
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getContext();

	public:
		/*各个状态的get/set方法*/
		void setBlendState(shared_ptr<BlendState>& state);
		shared_ptr<BlendState>& getBlendState();
		void setDepthStencilState(shared_ptr<DepthStencilState>& state);
		shared_ptr<DepthStencilState>& getDepthStencilState();
		void setRasterizerState(shared_ptr<RasterizerState>& state);
		shared_ptr<RasterizerState>& getRasterizerState();

	protected:
		HINSTANCE _ins;
		HWND _hwnd;
		int _width, _height;

		/*初始化dx需要的各种数据*/
		D3D_DRIVER_TYPE _driverType;
		D3D_FEATURE_LEVEL _featureLevel;
		ID3D11Device* _device;
		ID3D11DeviceContext* _context;
		IDXGISwapChain* _chain;

		/*缓存视图*/
		ID3D11Texture2D* _renderTargetBuffer;
		ID3D11RenderTargetView* _renderTargetView;

		/*模板视图*/
		ID3D11Texture2D* _depthStencilBuffer;
		ID3D11DepthStencilView* _depthStencilView;

	/*渲染管道的各种状态, 存储的是自定义的值, 分别指默认的各个状态和当前的状态*/
	protected:
		/*光栅化状态*/
		shared_ptr<RasterizerState> _defaultRasterState;
		shared_ptr<RasterizerState> _activeRasterState;
		/*混合状态*/
		shared_ptr<BlendState> _defaultBlendState;
		shared_ptr<BlendState> _activeBlendState;
		/*采样状态*/
		//ID3D11SamplerState* _sampleState;
		/*深度缓存状态*/
		shared_ptr<DepthStencilState> _defaultDepthStencilState;
		shared_ptr<DepthStencilState> _activeDepthStencilState;









		HRESULT compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint,
			LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

		ID3D11VertexShader* _vs = nullptr;
		ID3D11PixelShader*  _ps = nullptr;
		ID3D11InputLayout*  _vertexLayout = nullptr;
		ID3D11Buffer* _vertexBuff = nullptr;
		ID3D11Buffer* _indexBuff = nullptr;
		ID3D11Buffer* _constBuff = nullptr;
		/*对贴图资源引用的resource view, 如果一个面需要多张图, 那么我们的_resView就会是个数组*/
		int _resViewNum = 0;
		ID3D11ShaderResourceView* _resView[8];

	protected:
		/*初始化各个变量到默认值*/
		void init();
		void destroy();

		/*创建设备*/
		bool createDevice();
		/*创建深度缓存*/
		bool createDepthStencilView();
		/*创建渲染视图*/
		bool createRenderTargetlView();
		/*创建视口*/
		void createViewPort();
		/*创建默认的对象, 包含默认的渲染过程中的各个状态*/
		void createDefaultObjects();








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

		/*默认创建开启和关闭的blendstate*/
		void createAlphaBlendState();
		void enableAlphaBlend();
		void disableAlphaBlend();

		bool createSamplerState();
		bool createTexture(const wchar_t* path);

	};
}


