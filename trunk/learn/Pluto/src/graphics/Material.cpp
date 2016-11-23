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

void Material::SetVSConstBufferSize(int byteWidth){
	_vsConstByteWidth[_vsConstBufferNum] = byteWidth;
	_vsConstBufferNum++;
}

void Material::SetPSConstBufferSize(int byteWidth){
	_psConstByteWidth[_psConstBufferNum] = byteWidth;
	_psConstBufferNum++;
}