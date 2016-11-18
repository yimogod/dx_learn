#include <graphics/Material.h>

Material::Material(){
	_name = "ADS";
}

Material::Material(const std::string &name){
	_name = name;
}

Material::~Material(){
}
