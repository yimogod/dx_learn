#pragma once
#include <d3d11_1.h>

class ConstBuffer{
public:
	ConstBuffer();
	~ConstBuffer();

	bool CreateConstBuffer(ID3D11Device* device, int byteWidth, bool forVS);
	inline void BindConstBuff(ID3D11DeviceContext* context, UINT StartSlot, UINT NumBuffer);
	inline void UpdateConstBuff(ID3D11DeviceContext* context, const void* data);
private:
	ID3D11Buffer* _constBuff = nullptr;
	bool _forVS = true;;
};

inline void ConstBuffer::BindConstBuff(ID3D11DeviceContext* context, UINT StartSlot, UINT NumBuffers){
	if(_forVS){
		context->VSSetConstantBuffers(StartSlot, NumBuffers, &_constBuff);
	}else{
		context->PSSetConstantBuffers(StartSlot, NumBuffers, &_constBuff);
	}
}

inline void ConstBuffer::UpdateConstBuff(ID3D11DeviceContext* context, const void* data){
	context->UpdateSubresource(_constBuff, 0, nullptr, data, 0, 0);
}