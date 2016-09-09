#pragma once
#include <d3d11_1.h>

class IndexBuffer{
public:
	IndexBuffer();
	~IndexBuffer();

	bool CreateIndexBuffer(ID3D11Device *device, int* indexList, int indexNum);
	void BindIndexBuff(ID3D11DeviceContext* context);

	inline int GetIndexNum() const;
	inline ID3D11Buffer* GetDXObj() const;
private:
	ID3D11Buffer* _indexBuff = nullptr;

	int _indexNum = 0;
};

inline ID3D11Buffer* IndexBuffer::GetDXObj() const{
	return _indexBuff;
}

inline int IndexBuffer::GetIndexNum() const{
	return _indexNum;
}