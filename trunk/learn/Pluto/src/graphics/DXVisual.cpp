#include <graphics\DXVisual.h>

DXVisual::DXVisual(){}

DXVisual::~DXVisual(){}

bool DXVisual::Init(ID3D11Device* device){
	//创建 vertex shader
	bool result = _vs.CreateVertexShader(device, layout);
	if(!result)return false;

	//创建 pixel shader
	result = ps.CreatePixelShader(device);
	return result;

	//CreateVertexBuffer
	_vertexBuffer.CreateVertexBuffer(device, vertices, 100000);

	//createconst buffer
	_constBuffer.CreateConstBuffer(device, sizeof(ConstantBuffer));
}

void DXVisual::Draw(ID3D11DeviceContext* context){
	_vertexBuffer.BindVertexBuff(context, sizeof(Vertex));
	_vs->VSSetShader(context);

	_constBuffer.BindConstBuff(context, 0, 1);

	_ps->PSSetShader(context);
}