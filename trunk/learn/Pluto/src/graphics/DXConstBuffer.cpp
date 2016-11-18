#include <graphics/DXConstBuffer.h>

ConstBuffer::ConstBuffer(){
}

ConstBuffer::~ConstBuffer(){
}

bool ConstBuffer::CreateConstBuffer(ID3D11Device* device, int byteWidth, int slot, bool forVS){
	_forVS = forVS;
	_startSlot = slot;

	/*创建constant buff, 类似于uniform变量*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = NULL;
	bd.StructureByteStride = 0;

	HRESULT hr = device->CreateBuffer(&bd, nullptr, &_constBuff);
	if(FAILED(hr))return false;

	return true;
}

void ConstBuffer::BindConstBuff(ID3D11DeviceContext* context, UINT NumBuffers){
	if(_forVS){
		context->VSSetConstantBuffers(_startSlot, NumBuffers, &_constBuff);
	}else{
		context->PSSetConstantBuffers(_startSlot, NumBuffers, &_constBuff);
	}
}

void ConstBuffer::Reset(){
	if(_constBuff)_constBuff->Release();

	_constBuff = nullptr;
	_forVS = true;
	_startSlot = 0;

}