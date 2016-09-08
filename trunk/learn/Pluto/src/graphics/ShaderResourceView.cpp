#include <graphics/ShaderResourceView.h>
#include <util/DDSTextureLoader.h>

ShaderResourceView::ShaderResourceView(){
}

ShaderResourceView::~ShaderResourceView(){
}

bool ShaderResourceView::AddTexture(ID3D11Device* device, const wchar_t* path){
	HRESULT hr = DirectX::CreateDDSTextureFromFile(device, path, nullptr, &_resView[_resViewNum], 2048U);
	if(FAILED(hr))return false;
	_resViewNum++;
	return true;
}