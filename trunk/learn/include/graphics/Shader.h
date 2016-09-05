#pragma once
#include <d3d11_1.h>

class Shader{
public:
	Shader();
	Shader(const wchar_t* fileName, const char* entrytPoint, const char* shaderModel);

	~Shader();

public:
	//±‡“Îshader
	bool CompileShaderFromFile(ID3DBlob** ppBlobOut);

	bool CreateVertexShader(ID3D11Device* device, ID3D11VertexShader** vs,
		D3D11_INPUT_ELEMENT_DESC desc[], int num, ID3D11InputLayout** layout);
	bool CreatePixelShader(ID3D11Device* device, ID3D11PixelShader** ps);

private:
	const wchar_t* _fileName;
	const char* _entryPoint;
	const char* _shaderModel;
};
