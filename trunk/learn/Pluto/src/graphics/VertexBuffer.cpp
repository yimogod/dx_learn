#include <graphics/VertexBuffer.h>

VertexBuffer::VertexBuffer(){
}

VertexBuffer::~VertexBuffer(){
}

bool VertexBuffer::CreateVertexBuffer(ID3D11Device* device, char* vertices, int vertNum, int singleVertByte){
	_vertexNum = vertNum;

	/*设置buff desc*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = singleVertByte * vertNum;//数据总长度
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = NULL;
	bd.MiscFlags = NULL;
	bd.StructureByteStride = 0;

	/*设置 resource data*/
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	/*创建vertex buff*/
	HRESULT hr = device->CreateBuffer(&bd, &InitData, &_vertexBuff);
	if(FAILED(hr))return false;

	return true;
}

void VertexBuffer::Reset(){
	_vertexNum = 0;
	if(_vertexBuff)_vertexBuff->Release();
	_vertexBuff = nullptr;
}