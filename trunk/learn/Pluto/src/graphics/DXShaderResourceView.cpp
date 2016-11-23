#include <graphics/DXShaderResourceView.h>
#include <util/DDSTextureLoader.h>
#include <util/CUtil.h>

ShaderResourceView::ShaderResourceView(){
	for(int i = 0; i < 8; i++){
		_resView[i] = nullptr;
	}
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

void ShaderResourceView::AddTexturePath(const std::string &path){
	str_2_wstr(path, _texturePathList[_resViewNum]);
	_resViewNum++;
}

void ShaderResourceView::Reset(){
	_resViewNum = 0;
	for(int i = 0; i < 8; i++){
		_texturePathList[i] = L"";
		if(_resView[i])_resView[i]->Release();
		_resView[i] = nullptr;
	}
}