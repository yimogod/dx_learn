#pragma once
#include <graphics/DXInclude.h>

class PixelShader
{
public:
	PixelShader();
	~PixelShader();

protected:
	ID3D11PixelShader* _ps = nullptr;
};
