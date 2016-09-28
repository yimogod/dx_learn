#include <graphics/ShaderResourceView.h>
#include <util/DDSTextureLoader.h>

ShaderResourceView::ShaderResourceView(){
}

ShaderResourceView::~ShaderResourceView(){
}

bool ShaderResourceView::CreateTexture(ID3D11Device* device){
	HRESULT hr = S_OK;;
	for(int i = 0; i < _resViewNum; i++){
		const wchar_t* cpath = _texturePathList[i].c_str();
		hr = DirectX::CreateDDSTextureFromFile(device, cpath, nullptr, &_resView[i], 2048U);
		if(FAILED(hr))return false;
	}
	return true;
}

void ShaderResourceView::AddTexturePath(const std::wstring &path){
	_texturePathList[_resViewNum] = path;
	_resViewNum++;
}

void ShaderResourceView::Reset(){
	_resViewNum = 0;
	for(int i = 0; i < 8; i++){
		_texturePathList[i] = nullptr;
		_resView[i] = nullptr;
	}
}