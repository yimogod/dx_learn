#include <graphics/Material.h>

Material::Material(){
	_name = "ADS";
}

Material::Material(const std::string &name){
	_name = name;
}

Material::~Material(){
}

void Material::AddTexture(const std::wstring &path){
	_textureList[_textureNum].SetPath(path);
	_textureNum++;
}