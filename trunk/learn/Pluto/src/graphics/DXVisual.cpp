#include <graphics/DXVisual.h>

DXVisual::DXVisual(){}

DXVisual::~DXVisual(){}

bool DXVisual::Init(ID3D11Device* device, char* vertices, int vertexNum, int* indices, int indexNum){
	//创建 vertex shader
	bool result = _vs.CreateVertexShader(device, _layout);
	if(!result)return false;

	//创建 pixel shader
	result = _ps.CreatePixelShader(device);
	if(!result)return false;

	int singleVertexByte = _layout.GetTotalByte(0);
	result = _vertexBuffer[0].CreateVertexBuffer(device, vertices, vertexNum, singleVertexByte);
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
	_layout.BindVertexBuffer(context, _vertexBuffer);
	_indexBuffer.BindIndexBuff(context);

	_vs.VSSetShader(context);
	for(int i = 0; i < _vsConstBufferNum; i++){
		_vsConstBuffer[i].BindConstBuff(context, 1);
	}
	_ps.PSSetShader(context);
	for(int i = 0; i < _psConstBufferNum; i++){
		_psConstBuffer[i].BindConstBuff(context, 1);
	}

	//我们的visual自身已经有了num张图片, 将图片传入到Texture2D register
	int textureNum = _resView.GetResViewNum();
	if(textureNum > 0)
		_resView.BindShaderResource(context, 0);
	//如果有传入rtt, 则我们接着添加rtt
	//这是res view持有RTT渲染的结果
	if(resView != nullptr)
		context->PSSetShaderResources(textureNum, 1, &resView);

	_samplerState.BindSamplerState(context);

	if(_indexBuffer.useIndex){
		context->DrawIndexed(_indexBuffer.GetIndexNum(), 0, 0);
	}else{
		int vertNum = _vertexBuffer[0].GetVertexNum();
		if(_layout.HasInstance()){
			int insNum = _vertexBuffer[1].GetVertexNum();
			context->DrawInstanced(vertNum, insNum, 0, 0);
		}else{
			context->Draw(vertNum, 0);
		}
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

void DXVisual::UpdateShader(wchar_t* vsName, wchar_t* psName){
	//重设shader名称
	PreInitShader(vsName, psName);
}

void DXVisual::Reset(){
	_layout.Reset();
	for(int i = 0; i < 4; i++){
		_vertexBuffer[i].Reset();
	}

	_indexBuffer.Reset();

	_vsConstBufferNum = 0;
	for(int i = 0; i < 4; i++){
		_vsConstBuffer[i].Reset();
	}

	_psConstBufferNum = 0;
	for(int i = 0; i < 4; i++){
		_psConstBuffer[i].Reset();
	}

	_vs.Reset();
	_ps.Reset();
	_samplerState.Reset();
	_resView.Reset();
}