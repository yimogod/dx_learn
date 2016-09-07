#include <graphics/PixelShader.h>

PixelShader::PixelShader(){
}

PixelShader::PixelShader(const wchar_t* fileName, const char* entrytPoint, const char* shaderModel) :
	Shader(fileName, entrytPoint, shaderModel)
{

}

PixelShader::~PixelShader(){
}

bool PixelShader::CreatePixelShader(ID3D11Device* device){
	ID3DBlob* blob = nullptr;

	bool result = CompileShaderFromFile(&blob);
	if(!result){
		blob->Release();
		return false;
	}

	HRESULT hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &_ps);
	if(FAILED(hr)){
		blob->Release();
		return false;
	}

	blob->Release();
	return true;
}