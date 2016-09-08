#pragma once
#include <d3d11_1.h>

#include <graphics/VertexBuffer.h>
#include <graphics/IndexBuffer.h>
#include <graphics/ConstBuffer.h>
#include <graphics/VertexShader.h>
#include <graphics/PixelShader.h>

class DXVisual
{
public:
	DXVisual();
	DXVisual();
	~DXVisual();



	bool Init(ID3D11Device* device);
	void Draw(ID3D11DeviceContext* context);
private:
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	ConstBuffer _constBuffer;

	VertexShader _vs;
	PixelShader _ps;
};

