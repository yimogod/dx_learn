#include <graphics\DXVisual.h>

DXVisual::DXVisual(){}

DXVisual::~DXVisual(){}

bool DXVisual::Init(ID3D11Device* device, void* vertices, int vertexNum, int* indices, int indexNum){
	//创建 vertex shader
	bool result = _vs.CreateVertexShader(device, _layout);
	if(!result)return false;

	//创建 pixel shader
	result = _ps.CreatePixelShader(device);
	if(!result)return false;

	int singleVertexByte = _layout.GetTotalByte(0);
	result = _vertexBuffer.CreateVertexBuffer(device, vertices, vertexNum, singleVertexByte);
	if(!result)return false;

	if(indexNum > 0){
		result = _indexBuffer.CreateIndexBuffer(device, indices, indexNum);
		if(!result)return false;
	}else{
		_indexBuffer.useIndex = false;
	}

	int slot = 0;
	for(int i = 0; i < _vsConstBufferNum; i++){
		result = _vsConstBuffer[i].CreateConstBuffer(device, _vsConstByteWidth[i], slot, true);
		slot++;
		if(!result)return false;
	}

	for(int i = 0; i < _psConstBufferNum; i++){
		result = _psConstBuffer[i].CreateConstBuffer(device, _psConstByteWidth[i], slot, false);
		slot++;
		if(!result)return false;
	}

	result = _samplerState.CreateSamplerState(device);
	if(!result)return false;

	result = _resView.CreateTexture(device);
	if(!result)return false;

	return result;
}

void DXVisual::Draw(ID3D11DeviceContext* context, ID3D11ShaderResourceView* resView){
	_vertexBuffer.BindVertexBuff(context, _layout);
	_indexBuffer.BindIndexBuff(context);

	_vs.VSSetShader(context);
	for(int i = 0; i < _vsConstBufferNum; i++){
		_vsConstBuffer[i].BindConstBuff(context, 1);
	}
	_ps.PSSetShader(context);
	for(int i = 0; i < _psConstBufferNum; i++){
		_psConstBuffer[i].BindConstBuff(context, 1);
	}

	if(resView == nullptr){
		_resView.BindShaderResource(context, 0);
	}else{
		context->PSSetShaderResources(0, 1, &resView);
	}
	_samplerState.BindSamplerState(context);

	if(_indexBuffer.useIndex){
		context->DrawIndexed(_indexBuffer.GetIndexNum(), 0, 0);
	}else{
		context->Draw(_vertexBuffer.GetVertexNum(), 0);
	}
}


void DXVisual::PreSetVSConstBufferSize(int byteWidth){
	_vsConstByteWidth[_vsConstBufferNum] = byteWidth;
	_vsConstBufferNum++;
}

void DXVisual::PreSetPSConstBufferSize(int byteWidth){
	_psConstByteWidth[_psConstBufferNum] = byteWidth;
	_psConstBufferNum++;
}