#include <graphics/Texture.h>
#include <util/PlutoUtil.h>

Texture::Texture(){
}

Texture::Texture(const std::string &path){
	_path = PlutoUtil::GetFullPath(path);
}

Texture::~Texture(){
	_path = nullptr;
}

void Texture::SetPath(const std::string &path){
	_path = PlutoUtil::GetFullPath(path);
}

Texture2D::Texture2D() : Texture()
{
}

Texture2D::Texture2D(const std::string &path) : Texture(path)
{
}

Texture2D::~Texture2D()
{
}