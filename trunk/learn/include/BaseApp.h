#pragma once
#include <string>
#include <d3d11_1.h>
#include <dinput.h>
#include <math/algebra/Matrix4x4.h>
#include <graphics/Vertex.h>
#include <sys/SysEnv.h>
#include <graphics/Scene.h>

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

class BaseApp{
public:
	BaseApp();
	virtual ~BaseApp();

	bool init(HINSTANCE ins, HWND hwnd);
	void destroy();

	virtual bool loadContent();
	virtual void unloadContent();

	virtual void render();
	virtual void update();

protected:
	HINSTANCE _ins;
	HWND _hwnd;

	float _width, _height;

	D3D_DRIVER_TYPE _driverType;
	D3D_FEATURE_LEVEL _featureLevel;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;
	IDXGISwapChain* _chain;

	/*缓存视图*/
	ID3D11RenderTargetView* _backBuffView;

	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;

	ID3D11RasterizerState* _wireframeRS;
	ID3D11RasterizerState* _solidRS;
	ID3D11BlendState* _blendState;
	ID3D11SamplerState* _sampleState;

	HRESULT compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint,
		LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

protected:
	ID3D11VertexShader* _vs = nullptr;
	ID3D11PixelShader*  _ps = nullptr;
	ID3D11InputLayout*  _vertexLayout = nullptr;
	ID3D11Buffer* _vertexBuff = nullptr;
	ID3D11Buffer* _indexBuff = nullptr;
	ID3D11Buffer* _constBuff = nullptr;
	/*对贴图资源引用的resource view, 如果一个面需要多张图, 那么我们的_resView就会是个数组*/
	int _resViewNum = 0;
	ID3D11ShaderResourceView* _resView[8];

	bool createDevice();
	bool createShader(CreateShaderInfo vs, CreateShaderInfo ps, D3D11_INPUT_ELEMENT_DESC layout[], int numElements);
	bool createVertexBuffer(Vertex *vertices, int indexNum);
	bool createIndexBuffer(unsigned short* indexList, int indexNum);
	bool createConstBuffer(ID3D11Buffer** constBuff, int byteWidth);
	bool createRasterizerState(D3D11_FILL_MODE fillmode, ID3D11RasterizerState* rs);
	bool createBlendState();
	bool createSamplerState();
	bool createTexture(const wchar_t* path);
	bool createDXInput();

protected:
	/*dx input*/
	IDirectInput8* _inputDevice;
	IDirectInputDevice8* _mouse;
	IDirectInputDevice8* _keyborad;
	DIDATAFORMAT _mouseDataFormat;
	DIOBJECTDATAFORMAT _inputObjFormat[5];

	char _keyboardBuff[256];
	MouseState _mouseState;
	void acquireInput();
	bool isKeyDown(char keycode);
	bool isLMouseDown();
	bool isRMouseDown();
	void UpdatePosByRMouse(Camera* camera, float value);
	void UpdatePosByLMouse(Mesh* mesh, float value);
	
	Scene _scene;

	float _mouseX, _mouseY;
	bool _isRMouseDown = false;
	bool _isLMouseDown = false;

protected:
	SysEnv _env;

	string getAppRoot();
	string getFullPath(char* path);
	wstring getFullPathW(char* path);
};