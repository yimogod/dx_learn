#pragma once
#include <string>

class Texture
{
public:
	Texture(const std::string &path);
	~Texture();

protected:
	std::string _path;
};

class Texture2D : public Texture
{
public:
	Texture2D(const std::string &path);
	~Texture2D();
};
