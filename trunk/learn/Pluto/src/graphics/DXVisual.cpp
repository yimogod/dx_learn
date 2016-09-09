#include <graphics\DXVisual.h>

DXVisual::DXVisual(){}

DXVisual::~DXVisual(){}

bool DXVisual::Init(ID3D11Device* device, void* vertices, int vertexNum){
	_vertexNum = vertexNum;

	//创建 vertex shader
	bool result = _vs.CreateVertexShader(device, _layout);
	if(!result)return false;

	//创建 pixel shader
	result = _ps.CreatePixelShader(device);
	if(!result)return false;

	int singleVertexByte = _layout.GetTotalByte();
	result = _vertexBuffer.CreateVertexBuffer(device, vertices, vertexNum * singleVertexByte);
	if(!result)return false;

	result = _constBuffer.CreateConstBuffer(device, _constByteWidth);
	if(!result)return false;

	result = _samplerState.CreateSamplerState(device);
	if(!result)return false;

	result = _resView.CreateTexture(device);
	if(!result)return false;

	return result;
}

void DXVisual::Draw(ID3D11DeviceContext* context){
	_vertexBuffer.BindVertexBuff(context, _layout);
	_vs.VSSetShader(context);
	_constBuffer.BindConstBuff(context, 0, 1);
	_ps.PSSetShader(context);

	_resView.BindShaderResource(context, 0);
	_samplerState.BindSamplerState(context, 0, 1);

	context->Draw(_vertexNum, 0);
}