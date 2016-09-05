#pragma once
#include <graphics/DXInclude.h>

class Shader
{
public:
	Shader();
	~Shader();

	wchar_t* fileName;
	char* entryPoint;
	char* shaderModel;

public:
	//����shader
	int Shader::CompileShaderFromFile(wchar_t* szFileName,
		char* szEntryPoint, char* szShaderModel, ID3DBlob** ppBlobOut);
};
