#include <graphics/Material.h>

Material::Material(){
	_name = "ADS";
}

Material::Material(const std::string &name){
	_name = name;
}

Material::~Material(){
	_name = "";
	_textureNum = 0;
	_vsConstBufferNum = 0;
	_psConstBufferNum = 0;
}

void Material::AddTexture(const std::string &path){
	_textureList[_textureNum].SetPath(path);
	_textureNum++;
}

Texture2D* Material::GetTexture(int i){
	if(i >= _textureNum)return nullptr;
	return &_textureList[i];
}

void Material::Set_VS_CBufferSize(int byteWidth){
	_vsConstByteWidth[_vsConstBufferNum] = byteWidth;
	_vsConstBufferNum++;
}

void Material::Set_PS_CBufferSize(int byteWidth){
	_psConstByteWidth[_psConstBufferNum] = byteWidth;
	_psConstBufferNum++;
}

int Material::Get_VS_CBufferSize(int i) const{
	if(i >= _vsConstBufferNum)return 0;
	return _vsConstByteWidth[i];
}

int Material::Get_PS_CBufferSize(int i) const{
	if(i >= _psConstBufferNum)return 0;
	return _psConstByteWidth[i];
}
