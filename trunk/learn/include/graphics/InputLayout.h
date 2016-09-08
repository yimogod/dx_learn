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