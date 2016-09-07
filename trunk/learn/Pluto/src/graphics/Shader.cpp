#include <d3dcompiler.h>
#include <graphics/Shader.h>

Shader::Shader()
{
}

Shader::Shader(const wchar_t* fileName, const char* entrytPoint, const char* shaderModel):
	_fileName(fileName), _entryPoint(entrytPoint), _shaderModel(shaderModel)
{

}


Shader::~Shader()
{
}

bool Shader::CompileShaderFromFile(ID3DBlob** ppBlobOut){
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	//#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	//#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(_fileName, nullptr, nullptr, _entryPoint, _shaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if(FAILED(hr) && pErrorBlob){
		OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
	}
	if(pErrorBlob){
		pErrorBlob->Release();
		return false;
	}
	if(FAILED(hr)){
		return false;
	}

	return true;
}

bool Shader::CreateVertexShader(ID3D11Device* device, ID3D11VertexShader** vs, InputLayout &inputLayout){
	ID3DBlob* blob = nullptr;

	bool result = CompileShaderFromFile(&blob);
	if(!result){
		blob->Release();
		return false;
	}

	HRESULT hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vs);
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

bool Shader::CreatePixelShader(ID3D11Device* device, ID3D11PixelShader** ps){
	ID3DBlob* blob = nullptr;

	bool result = CompileShaderFromFile(&blob);
	if(!result){
		blob->Release();
		return false;
	}

	HRESULT hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, ps);
	if(FAILED(hr)){
		blob->Release();
		return false;
	}

	blob->Release();
	return true;
}