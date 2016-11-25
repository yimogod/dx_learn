#pragma once
#include <string>
#include <BaseDataStruct.h>
#include <graphics/Texture.h>
#include <util/CUtil.h>

class Material
{
public:
	Material();
	Material(const std::string &name);
	~Material();

	inline void SetName(const std::string &name);
	inline std::string& GetName();
	inline std::wstring GetName_w();
	inline void SetColor(Color &color);
	inline Color GetColor();

	void AddTexture(const std::string &path);
	Texture2D* GetTexture(int i);
	inline int GetTextureNum() const;

	void Set_VS_CBufferSize(int byteWidth);//设置constbuffer对象的byte
	void Set_PS_CBufferSize(int byteWidth);//设置constbuffer对象的byte
	int Get_VS_CBufferSize(int i) const;
	int Get_PS_CBufferSize(int i) const;
	
	//这只要传给dx需要的const 数据
	void Set_VS_CBufferData(int i, char* data);
	void Set_PS_CBufferData(int i, char* data);
	const char* Get_VS_CBufferData(int i) const;
	const char* Get_PS_CBufferData(int i) const;

	inline int Get_VS_CBufferNum() const;
	inline int Get_PS_CBufferNum() const;
private:
	std::string _name;
	//material身上的颜色
	Color _color;

	int _textureNum = 0;
	Texture2D _textureList[4];

	//For DX
	char* _vsConstData[4];
	int _vsConstByteWidth[4];
	int _vsConstBufferNum = 0;

	//ps buffer对应的数据, 在cpu端赋值后, 要传给dx
	char* _psConstData[4];
	int _psConstByteWidth[4];
	int _psConstBufferNum = 0;
};

inline void Material::SetName(const std::string &name){
	_name = name;
}

inline std::string& Material::GetName(){
	return _name;
}

inline std::wstring Material::GetName_w(){
	std::wstring wname;
	str_2_wstr(_name, wname);
	return wname;
}

inline void Material::SetColor(Color &color){
	_color = color;
}

inline Color Material::GetColor(){
	return _color;
}

inline int Material::GetTextureNum() const{
	return _textureNum;
}

inline int Material::Get_VS_CBufferNum() const{
	return _vsConstBufferNum;
}

inline int Material::Get_PS_CBufferNum() const{
	return _psConstBufferNum;
}