#pragma once
#include <d3d11_1.h>
#include <graphics/DXInputLayout.h>

class Shader{
public:
	Shader();
	Shader(wchar_t* fileName, char* entrytPoint, char* shaderModel);
	virtual ~Shader();

	virtual void Reset();
	inline void SetFileName(wchar_t* fileName);
protected:
	//±‡“Îshader
	bool CompileShaderFromFile(ID3DBlob** ppBlobOut);

protected:
	wchar_t* _fileName = nullptr;
	char* _entryPoint = nullptr;
	char* _shaderModel = nullptr;
};


inline void Shader::SetFileName(wchar_t* fileName){
	_fileName = fileName;
}
