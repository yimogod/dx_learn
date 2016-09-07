#pragma once
#include <d3d11_1.h>
#include <graphics/InputLayout.h>

class Shader{
public:
	Shader();
	Shader(const wchar_t* fileName, const char* entrytPoint, const char* shaderModel);

	virtual ~Shader();
protected:
	//±‡“Îshader
	bool CompileShaderFromFile(ID3DBlob** ppBlobOut);

protected:
	const wchar_t* _fileName;
	const char* _entryPoint;
	const char* _shaderModel;
};
