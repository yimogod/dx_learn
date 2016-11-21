#pragma once
#include <string>
#include <scene/Mesh.h>

class PlutoUtil
{
public:
	PlutoUtil();
	~PlutoUtil();


	static std::string const GetAppRoot();
	static std::string GetFullPath(char* path);
	static std::wstring GetFullPathW(const char* path);
	static void AddTexture(Mesh* mesh, const char* texturePath);//º”‘ÿ∂‡’≈Õº∆¨
};
