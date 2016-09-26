#pragma once
#include <d3d11_1.h>
#include <graphics/InputLayout.h>

class VertexBuffer{
public:
	VertexBuffer();
	~VertexBuffer();

	bool CreateVertexBuffer(ID3D11Device* device, void* vertices, int vertNum, int singleVertByte);
	void BindVertexBuff(ID3D11DeviceContext* context, InputLayout &layout, int slot = 0);
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