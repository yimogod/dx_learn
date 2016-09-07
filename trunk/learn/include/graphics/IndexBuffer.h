#pragma once
#include <d3d11_1.h>

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	bool CreateIndexBuffer(ID3D11Device *device, unsigned short* indexList, int indexNum);
	void BindIndexBuff(ID3D11DeviceContext* context);

private:
	ID3D11Buffer* _indexBuff = nullptr;
};