#include <graphics/PixelShader.h>

PixelShader::PixelShader() : Shader(L"DummyShader", "PS", "ps_4_0")
{
}

PixelShader::PixelShader(wchar_t* fileName) : Shader(fileName, "PS", "ps_4_0")
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