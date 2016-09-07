#include <graphics/InputLayout.h>

InputLayout::InputLayout(){
}

InputLayout::~InputLayout(){
}

void InputLayout::AddElement(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, int byteWidth){
	_layout[_elementNum].SemanticName = SemanticName;
	_layout[_elementNum].SemanticIndex = SemanticIndex;
	_layout[_elementNum].Format = Format;
	_layout[_elementNum].InputSlot = 0;
	_layout[_elementNum].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	_layout[_elementNum].InstanceDataStepRate = 0;
	_layout[_elementNum].AlignedByteOffset = _totalByte;

	_elementNum++;
	_totalByte += byteWidth;
}

HRESULT InputLayout::CreateInputLayout(ID3D11Device* device, ID3DBlob* blob){
	HRESULT hr = device->CreateInputLayout(_layout, _elementNum,
		blob->GetBufferPointer(), blob->GetBufferSize(), &_vertexLayout);
	return hr;
}