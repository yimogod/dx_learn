#include <graphics/ShaderResourceView.h>
#include <util/DDSTextureLoader.h>

ShaderResourceView::ShaderResourceView(){
}

ShaderResourceView::~ShaderResourceView(){
}

bool ShaderResourceView::CreateTexture(ID3D11Device* device){
	HRESULT hr = S_OK;;
	for(int i = 0; i < _resViewNum; i++){
		hr = DirectX::CreateDDSTextureFromFile(device, _texturePathList[i], nullptr, &_resView[i], 2048U);
		if(FAILED(hr))return false;
	}
	return true;
}

void ShaderResourceView::AddTexturePath(const wchar_t* path){
	_texturePathList[_resViewNum] = path;
	_resViewNum++;
}