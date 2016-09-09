#include <graphics\IndexBuffer.h>

IndexBuffer::IndexBuffer()
{
}


IndexBuffer::~IndexBuffer()
{
}

/*����index buff*/
bool IndexBuffer::CreateIndexBuffer(ID3D11Device *device, int* indexList, int indexNum){
	_indexNum = indexNum;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum;//��������ռ�õ����пռ�
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = NULL;

	/*���� resource data*/
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indexList;
	HRESULT hr = device->CreateBuffer(&bd, &InitData, &_indexBuff);
	if(FAILED(hr))return false;
	return true;
}

void IndexBuffer::BindIndexBuff(ID3D11DeviceContext* context){
	context->IASetIndexBuffer(_indexBuff, DXGI_FORMAT_R16_UINT, 0);
}