#pragma once
#include <DirectXMath.h>
#include "BaseApp.h"

using namespace DirectX;

struct VertexPos{
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
};

class DemoApp1 : public BaseApp{
public:
	DemoApp1();
	virtual ~DemoApp1();

	bool loadContent();
	void unloadContent();

	void update();
	void render();

private:
	ID3D11VertexShader* _vs = nullptr;
	ID3D11PixelShader*  _ps = nullptr;
	ID3D11InputLayout*  _vertexLayout = nullptr;
	ID3D11Buffer* _vertexBuff = nullptr;
	ID3D11ShaderResourceView* _resView;
	ID3D11SamplerState* _sampleState;

	bool createShader();
	bool createVertexBuffer();
	bool createTexture();
};