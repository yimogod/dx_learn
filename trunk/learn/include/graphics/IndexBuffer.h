#pragma once
#include <d3d11_1.h>

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	bool CreateIndexBuffer(ID3D11Device *device, unsigned short* indexList, int indexNum);
	void BindIndexBuff(ID3D11DeviceContext* context);
	inline ID3D11Buffer* GetDXObj() const;
private:
	ID3D11Buffer* _indexBuff = nullptr;
};

inline ID3D11Buffer* IndexBuffer::GetDXObj() const{
	return _indexBuff;
}
