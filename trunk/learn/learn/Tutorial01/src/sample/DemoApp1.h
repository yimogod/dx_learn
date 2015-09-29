#pragma once
#include <DirectXMath.h>
#include "BaseApp.h"

using namespace DirectX;

struct SimpleVertex{
	XMFLOAT3 Pos;
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

	bool createShader();
	bool createVertexBuffer();
};