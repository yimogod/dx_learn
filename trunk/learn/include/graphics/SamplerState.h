#pragma once
#include <d3d11_1.h>

class SamplerState{
public:
	SamplerState();
	~SamplerState();

	inline int GetStateNum() const;

	bool CreateSamplerState(ID3D11Device* device);
	void BindSamplerState(ID3D11DeviceContext* context);
	void AddAdress(D3D11_TEXTURE_ADDRESS_MODE adress);
private:
	//一个visual会用到多个sampler
	int _stateNum = 0;
	D3D11_TEXTURE_ADDRESS_MODE _adress[8];
	ID3D11SamplerState* _samplerState[8];
};

inline int SamplerState::GetStateNum() const{
	return _stateNum;
}