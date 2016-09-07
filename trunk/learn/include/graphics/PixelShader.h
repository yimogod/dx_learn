#pragma once
#include <graphics/Shader.h>
class PixelShader : public Shader
{
public:
	PixelShader();
	PixelShader(const wchar_t* fileName, const char* entrytPoint, const char* shaderModel);
	~PixelShader();

	bool CreatePixelShader(ID3D11Device* device);

private:
	ID3D11PixelShader* _ps;
};
