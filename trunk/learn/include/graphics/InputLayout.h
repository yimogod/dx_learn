#pragma once
#include <d3d11_1.h>
#include <graphics/VertexBuffer.h>

class InputLayout{
public:
	InputLayout();
	~InputLayout();

public:
	void AddElement(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, int byteWidth);
	HRESULT CreateInputLayout(ID3D11Device* device, ID3DBlob* blob);
	//绑定1个/多个 vertextbuffer到此layout下
	void BindVertexBuffer(ID3D11DeviceContext* context, VertexBuffer* vertexList);

	inline void AddPosition();
	inline void AddNormal();
	inline void AddTangent();
	inline void AddColor();
	inline void AddTexCoord();
	void AddInstance();

	inline int GetElementNum() const;
	inline int GetTotalByte(int slot) const;
	inline int GetSlotNum() const;
	inline ID3D11InputLayout* GetDXObj() const;

private:
	ID3D11InputLayout* _vertexLayout = nullptr;
	D3D11_INPUT_ELEMENT_DESC _layout[8];
	int _elementNum = 0;

	//占用了几个slot
	int _slotIndex = 0;
	int _totalByte[4];

	//当前texture语义索引
	int _semanTextureIndex = 0;

};

inline int InputLayout::GetElementNum() const{
	return _elementNum;
}

inline int InputLayout::GetTotalByte(int slot) const{
	return _totalByte[slot];
}

inline int InputLayout::GetSlotNum() const{
	return _slotIndex + 1;
}

inline ID3D11InputLayout* InputLayout::GetDXObj() const{
	return _vertexLayout;
}

inline void InputLayout::AddPosition(){
	AddElement("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
}

inline void InputLayout::AddNormal(){
	AddElement("NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
}

inline void InputLayout::AddTangent(){
	AddElement("TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
}

inline void InputLayout::AddColor(){
	AddElement("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
}

inline void InputLayout::AddTexCoord(){
	AddElement("TEXCOORD", _semanTextureIndex, DXGI_FORMAT_R32G32_FLOAT, 8);
	_semanTextureIndex++;
}