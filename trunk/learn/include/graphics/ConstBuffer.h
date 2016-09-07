#pragma once
#include <d3d11_1.h>

class ConstBuffer{
public:
	ConstBuffer();
	~ConstBuffer();

	bool CreateConstBuffer(ID3D11Device* device, int byteWidth);
private:
	ID3D11Buffer* _constBuff = nullptr;
};

