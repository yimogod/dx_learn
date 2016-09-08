#pragma once
#include <d3d11_1.h>

class InputLayout
{
public:
	InputLayout();
	~InputLayout();

public:
	void AddElement(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, int byteWidth);
	HRESULT CreateInputLayout(ID3D11Device* device, ID3DBlob* blob);

	inline void AddPosition();
	inline void AddColor();
	inline void AddTexCoord();
	inline void AddTexCoord_1();

	inline int GetElementNum() const;
	inline int GetTotalByte() const;
	inline ID3D11InputLayout* GetDXObj() const;

private:
	ID3D11InputLayout* _vertexLayout = nullptr;
	D3D11_INPUT_ELEMENT_DESC _layout[8];
	int _elementNum = 0;
	int _totalByte = 0;
};

inline int InputLayout::GetElementNum() const{
	return _elementNum;
}

inline int InputLayout::GetTotalByte() const{
	return _totalByte;
}

inline ID3D11InputLayout* InputLayout::GetDXObj() const{
	return _vertexLayout;
}

inline void InputLayout::AddPosition(){
	AddElement("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
}

inline void InputLayout::AddColor(){
	AddElement("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
}

inline void InputLayout::AddTexCoord(){
	AddElement("TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
}

inline void InputLayout::AddTexCoord_1(){
	AddElement("TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
}