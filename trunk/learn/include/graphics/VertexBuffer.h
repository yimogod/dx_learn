#pragma once
#include <d3d11_1.h>
#include <graphics/InputLayout.h>

class VertexBuffer{
public:
	VertexBuffer();
	~VertexBuffer();

	bool CreateVertexBuffer(ID3D11Device* device, void* vertices, int totalByte);
	void BindVertexBuff(ID3D11DeviceContext* context, InputLayout &layout);
	inline ID3D11Buffer* GetDXObj() const;
private:
	ID3D11Buffer* _vertexBuff = nullptr;
};

inline ID3D11Buffer* VertexBuffer::GetDXObj() const{
	return _vertexBuff;
}
