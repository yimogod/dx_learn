#include <graphics\SamplerState.h>

SamplerState::SamplerState(){
	_stateNum = 0;
	for(int i = 0; i < 8; i++){
		_adress[i] = D3D11_TEXTURE_ADDRESS_WRAP;
	}
}

SamplerState::~SamplerState(){}

void SamplerState::AddAdress(D3D11_TEXTURE_ADDRESS_MODE adress){
	_adress[_stateNum] = adress;
	_stateNum++;
}

bool SamplerState::CreateSamplerState(ID3D11Device* device){
	//简化代码, 如果没有副职samplerstate, 我们默认创建一个
	if(_stateNum == 0)AddAdress(D3D11_TEXTURE_ADDRESS_WRAP);

	for(int i = 0; i < _stateNum; i++){
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = _adress[i];
		sampDesc.AddressV = _adress[i];
		sampDesc.AddressW = _adress[i];
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		HRESULT hr = device->CreateSamplerState(&sampDesc, &_samplerState[i]);
		if(FAILED(hr))return false;
	}

	return true;
}

void SamplerState::BindSamplerState(ID3D11DeviceContext* context){
	for(int i = 0; i < _stateNum; i++){
		context->PSSetSamplers(i, 1, &_samplerState[i]);
	}
}