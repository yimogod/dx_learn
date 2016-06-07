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

protected:
	//±‡“Îshader
	int Shader::compileShaderFromFile(wchar_t* szFileName,
		char* szEntryPoint, char* szShaderModel, ID3DBlob** ppBlobOut);
};
