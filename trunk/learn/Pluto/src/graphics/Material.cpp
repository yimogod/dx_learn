#include <graphics/Material.h>
#include <iostream>

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
	_vsConstData[_vsConstBufferNum] = new char[byteWidth];
	_vsConstBufferNum++;
}

void Material::Set_PS_CBufferSize(int byteWidth){
	_psConstByteWidth[_psConstBufferNum] = byteWidth;
	_psConstData[_vsConstBufferNum] = new char[byteWidth];
	_psConstBufferNum++;
}

int Material::Get_VS_CBufferSize(int i) const{
	if(i >= _vsConstBufferNum){
		std::cout << "Get_VS_CBufferSize--> i bigger then max num" << std::endl;
		return 0;
	}
	return _vsConstByteWidth[i];
}

int Material::Get_PS_CBufferSize(int i) const{
	if(i >= _psConstBufferNum){
		std::cout << "Get_PS_CBufferSize--> i bigger then max num" << std::endl;
		return 0;
	}
	return _psConstByteWidth[i];
}

void Material::Set_VS_CBufferData(int i, char* data){
	if(i >= _vsConstBufferNum){
		std::cout << "Set_VS_CBufferData--> i bigger then max num" << std::endl;
		return;
	}

	//¿½±´Êý¾Ý
	memcpy(_vsConstData[i], data, _vsConstByteWidth[i]);
	//_vsConstData[i] = data;
}

void Material::Set_PS_CBufferData(int i, char* data){
	if(i >= _psConstBufferNum){
		std::cout << "Set_PS_CBufferData--> i bigger then max num" << std::endl;
		return;
	}

	memcpy(_psConstData[i], data, _psConstByteWidth[i]);
}

const char* Material::Get_VS_CBufferData(int i) const{
	if(i >= _vsConstBufferNum){
		std::cout << "Get_VS_CBufferData--> i bigger then max num" << std::endl;
		return nullptr;
	}

	return _vsConstData[i];
}

const char* Material::Get_PS_CBufferData(int i) const{
	if(i >= _psConstBufferNum){
		std::cout << "Get_PS_CBufferData--> i bigger then max num" << std::endl;
		return nullptr;
	}

	return _psConstData[i];
}