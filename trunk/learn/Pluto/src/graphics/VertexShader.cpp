#include <graphics/VertexShader.h>

VertexShader::VertexShader(){
}

VertexShader::VertexShader(const wchar_t* fileName, const char* entrytPoint, const char* shaderModel):
	Shader(fileName, entrytPoint, shaderModel)
{

}

VertexShader::~VertexShader(){
}

bool VertexShader::CreateVertexShader(ID3D11Device* device, InputLayout &inputLayout){
	ID3DBlob* blob = nullptr;

	bool result = CompileShaderFromFile(&blob);
	if(!result){
		blob->Release();
		return false;
	}

	HRESULT hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &_vs);
	if(FAILED(hr)){
		blob->Release();
		return false;
	}

	hr = inputLayout.CreateInputLayout(device, blob);
	if(FAILED(hr)){
		blob->Release();
		return false;
	}

	blob->Release();
	return true;
}