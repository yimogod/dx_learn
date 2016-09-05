#pragma once
#include <d3d11_1.h>

class Shader{
public:
	Shader();
	Shader(const wchar_t* fileName, const char* entrytPoint, const char* shaderModel);

	~Shader();

public:
	//±‡“Îshader
	int Shader::CompileShaderFromFile(ID3DBlob** ppBlobOut);

private:
	const wchar_t* _fileName;
	const char* _entryPoint;
	const char* _shaderModel;
};
