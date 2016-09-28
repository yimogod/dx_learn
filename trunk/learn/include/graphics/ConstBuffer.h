#pragma once
#include <d3d11_1.h>

class ConstBuffer{
public:
	ConstBuffer();
	~ConstBuffer();

	void Reset();
	bool CreateConstBuffer(ID3D11Device* device, int byteWidth, int slot, bool forVS);
	void BindConstBuff(ID3D11DeviceContext* context, UINT NumBuffer);
	inline void UpdateConstBuff(ID3D11DeviceContext* context, const void* data);
private:
	ID3D11Buffer* _constBuff = nullptr;
	bool _forVS = true;
	int _startSlot = 0;
};

inline void ConstBuffer::UpdateConstBuff(ID3D11DeviceContext* context, const void* data){
	context->UpdateSubresource(_constBuff, 0, nullptr, data, 0, 0);
}