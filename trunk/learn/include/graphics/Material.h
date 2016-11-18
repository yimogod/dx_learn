#pragma once
#include <string>
#include <BaseDataStruct.h>


class Material
{
public:
	Material();
	Material(const std::string &name);
	~Material();

	inline void SetName(const std::string &name);
	inline std::string GetName();
	inline void SetColor(Color &color);
	inline Color GetColor();

private:
	std::string _name;
	//material身上的颜色
	Color _color;
};

inline void Material::SetName(const std::string &name){
	_name = name;
}

inline std::string Material::GetName(){
	return _name;
}

inline void Material::SetColor(Color &color){
	_color = color;
}

inline Color Material::GetColor(){
	return _color;
}