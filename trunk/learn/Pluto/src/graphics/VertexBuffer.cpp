#include <graphics\VertexBuffer.h>

VertexBuffer::VertexBuffer(){
}

VertexBuffer::~VertexBuffer(){
}

bool VertexBuffer::CreateVertexBuffer(ID3D11Device* device, void* vertices, int totalByte){
	/*����buff desc*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = totalByte;//�����ܳ���
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = NULL;

	/*���� resource data*/
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	/*����vertex buff*/
	HRESULT hr = device->CreateBuffer(&bd, &InitData, &_vertexBuff);
	if(FAILED(hr))return false;

	return true;
}


void VertexBuffer::BindVertexBuff(ID3D11DeviceContext* context, InputLayout &layout){
	/*���� layout*/
	context->IASetInputLayout(layout.GetDXObj());

	/*���õ�ǰvertex buff*/
	UINT stride = layout.GetTotalByte();// sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &_vertexBuff, &stride, &offset);

	/*�������������з�ʽ, ���������˷�ʽ*/
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}