#pragma once
#include <graphics/Shader.h>
class VertexShader : public Shader{
public:
	VertexShader();
	VertexShader(const wchar_t* fileName, const char* entrytPoint, const char* shaderModel);

	~VertexShader();
	
	bool CreateVertexShader(ID3D11Device* device, InputLayout &inputLayout);
	
private:
	ID3D11VertexShader* _vs;
};

