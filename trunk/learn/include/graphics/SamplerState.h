#pragma once
#include <d3d11_1.h>

class SamplerState{
public:
	SamplerState();
	~SamplerState();

	bool CreateSamplerState(ID3D11Device* device);
	inline void BindSamplerState(ID3D11DeviceContext* context, UINT StartSlot, UINT NumSamplers);

private:
	ID3D11SamplerState* _samplerState;
};

inline void SamplerState::BindSamplerState(ID3D11DeviceContext* context, UINT StartSlot, UINT NumSamplers){
	context->PSSetSamplers(StartSlot, NumSamplers, &_samplerState);
}