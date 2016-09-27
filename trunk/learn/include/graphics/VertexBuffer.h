#pragma once
#include <d3d11_1.h>

class VertexBuffer{
public:
	VertexBuffer();
	~VertexBuffer();

	bool CreateVertexBuffer(ID3D11Device* device, char* vertices, int vertNum, int singleVertByte);
	inline int GetVertexNum() const;
	inline ID3D11Buffer* GetDXObj() const;
private:
	ID3D11Buffer* _vertexBuff = nullptr;
	int _vertexNum = 0;
};

inline ID3D11Buffer* VertexBuffer::GetDXObj() const{
	return _vertexBuff;
}

inline int VertexBuffer::GetVertexNum() const{
	return _vertexNum;
}