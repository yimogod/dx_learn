#pragma once
#include <string>

class Texture
{
public:
	Texture();
	Texture(const std::string &path);
	~Texture();

	inline void SetPath(const std::string &path);

protected:
	std::string _path;
};

inline void Texture::SetPath(const std::string &path){
	_path = path;
}

class Texture2D : public Texture
{
public:
	Texture2D();
	Texture2D(const std::string &path);
	~Texture2D();
};
