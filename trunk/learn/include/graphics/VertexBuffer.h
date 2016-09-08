#pragma once
#include <d3d11_1.h>

class VertexBuffer{
public:
	VertexBuffer();
	~VertexBuffer();

	bool CreateVertexBuffer(ID3D11Device* device, void* vertices, int totalByte);
	void BindVertexBuff(ID3D11DeviceContext* context, UINT vertexByte);
private:
	ID3D11Buffer* _vertexBuff = nullptr;
};
