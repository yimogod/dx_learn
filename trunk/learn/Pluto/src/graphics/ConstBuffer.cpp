#include <graphics\ConstBuffer.h>

ConstBuffer::ConstBuffer()
{
}


ConstBuffer::~ConstBuffer()
{
}

bool ConstBuffer::CreateConstBuffer(ID3D11Device* device, int byteWidth){
	/*创建constant buff, 类似于uniform变量*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = NULL;

	HRESULT hr = device->CreateBuffer(&bd, nullptr, &_constBuff);
	if(FAILED(hr))return false;

	return true;
}