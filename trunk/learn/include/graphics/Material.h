#pragma once
#include <string>
#include <BaseDataStruct.h>
#include <graphics/Texture.h>

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

	void AddTexture(const std::wstring &path);

private:
	std::string _name;
	//material身上的颜色
	Color _color;

	int _textureNum = 0;
	Texture2D _textureList[4];
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