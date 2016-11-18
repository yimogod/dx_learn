#include "graphics/Texture.h"

Texture::Texture()
{
}

Texture::Texture(const std::wstring &path){
	_path = path;
}

Texture::~Texture()
{
}

Texture2D::Texture2D() : Texture()
{
}

Texture2D::Texture2D(const std::wstring &path) : Texture(path)
{
}

Texture2D::~Texture2D()
{
}