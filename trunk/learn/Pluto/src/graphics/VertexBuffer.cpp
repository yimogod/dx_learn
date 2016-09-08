#include <graphics\VertexBuffer.h>

VertexBuffer::VertexBuffer(){
}

VertexBuffer::~VertexBuffer(){
}

bool VertexBuffer::CreateVertexBuffer(ID3D11Device* device, void* vertices, int totalByte){
	/*设置buff desc*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = totalByte;//数据总长度
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = NULL;

	/*设置 resource data*/
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	/*创建vertex buff*/
	HRESULT hr = device->CreateBuffer(&bd, &InitData, &_vertexBuff);
	if(FAILED(hr))return false;

	return true;
}


void VertexBuffer::BindVertexBuff(ID3D11DeviceContext* context, InputLayout &layout){
	/*设置 layout*/
	context->IASetInputLayout(layout.GetDXObj());

	/*设置当前vertex buff*/
	UINT stride = layout.GetTotalByte();// sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &_vertexBuff, &stride, &offset);

	/*设置三角形排列方式, 即顶点拓扑方式*/
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}