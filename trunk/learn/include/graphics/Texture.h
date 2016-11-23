#pragma once
#include <string>

class Texture
{
public:
	Texture();
	Texture(const std::string &path);
	~Texture();

	void SetPath(const std::string &path);
	inline std::string& GetPath();
protected:
	std::string _path;
};

inline std::string& Texture::GetPath(){
	return _path;
}


class Texture2D : public Texture
{
public:
	Texture2D();
	Texture2D(const std::string &path);
	~Texture2D();
};
