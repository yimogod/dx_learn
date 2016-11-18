#pragma once
#include <string>

class Texture
{
public:
	Texture();
	Texture(const std::wstring &path);
	~Texture();

	inline void SetPath(const std::wstring &path);

protected:
	std::wstring _path;
};

inline void Texture::SetPath(const std::wstring &path){
	_path = path;
}

class Texture2D : public Texture
{
public:
	Texture2D();
	Texture2D(const std::wstring &path);
	~Texture2D();
};
