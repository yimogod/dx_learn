#pragma once
#include <d3d11_1.h>

class ConstBuffer{
public:
	ConstBuffer();
	~ConstBuffer();

	bool CreateConstBuffer(ID3D11Device* device, int byteWidth);
	inline void BindConstBuff(ID3D11DeviceContext* context, UINT StartSlot, UINT NumBuffer);
	inline ID3D11Buffer* GetDXObj() const;
private:
	ID3D11Buffer* _constBuff = nullptr;
};

inline void ConstBuffer::BindConstBuff(ID3D11DeviceContext* context, UINT StartSlot, UINT NumBuffers){
	context->VSSetConstantBuffers(StartSlot, NumBuffers, &_constBuff);
}

inline ID3D11Buffer* ConstBuffer::GetDXObj() const{
	return _constBuff;
}