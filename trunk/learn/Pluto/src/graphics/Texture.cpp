#include "graphics/Texture.h"

Texture::Texture(const std::string &path){
	_path = path;
}

Texture::~Texture()
{
}

Texture2D::Texture2D(const std::string &path) : Texture(path)
{
}

Texture2D::~Texture2D()
{
}