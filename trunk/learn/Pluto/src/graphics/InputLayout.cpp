#include <graphics/InputLayout.h>

InputLayout::InputLayout(){
	_totalByte[0] = 0;
	_totalByte[1] = 0;
	_totalByte[2] = 0;
	_totalByte[3] = 0;
}

InputLayout::~InputLayout(){
}

void InputLayout::AddElement(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, int byteWidth){
	_layout[_elementNum].SemanticName = SemanticName;
	_layout[_elementNum].SemanticIndex = SemanticIndex;
	_layout[_elementNum].Format = Format;
	_layout[_elementNum].InputSlot = _slotIndex;
	_layout[_elementNum].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	_layout[_elementNum].InstanceDataStepRate = 0;
	_layout[_elementNum].AlignedByteOffset = _totalByte[_slotIndex];

	_elementNum++;
	_totalByte[_slotIndex] += byteWidth;
}

void InputLayout::AddInstance(){
	//添加instance数据, 需要开辟新的slot
	_slotIndex++;

	_layout[_elementNum].SemanticName = "TEXCOORD";
	_layout[_elementNum].SemanticIndex = _semanTextureIndex;
	_layout[_elementNum].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	_layout[_elementNum].InputSlot = _slotIndex;
	_layout[_elementNum].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	_layout[_elementNum].InstanceDataStepRate = 1;
	_layout[_elementNum].AlignedByteOffset = 0;

	_elementNum++;
	_totalByte[_slotIndex] += 16;
}

HRESULT InputLayout::CreateInputLayout(ID3D11Device* device, ID3DBlob* blob){
	HRESULT hr = device->CreateInputLayout(_layout, _elementNum,
		blob->GetBufferPointer(), blob->GetBufferSize(), &_vertexLayout);
	return hr;
}

void InputLayout::BindVertexBuffer(ID3D11DeviceContext* context, VertexBuffer* vertexList){
	/*设置 layout*/
	context->IASetInputLayout(_vertexLayout);
	int slotNum = GetSlotNum();
	UINT* strides = new UINT[slotNum];
	UINT* offsets = new UINT[slotNum];
	ID3D11Buffer** bufferPointers = new ID3D11Buffer*[slotNum];

	for(int i = 0; i < slotNum; i++){
		strides[i] = GetTotalByte(i);
		offsets[i] = 0;
		bufferPointers[i] = vertexList[i].GetDXObj();
	}
	context->IASetVertexBuffers(0, slotNum, bufferPointers, strides, offsets);

	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	/*设置三角形排列方式, 即顶点拓扑方式*/
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	delete[] strides;
	delete[] offsets;
}