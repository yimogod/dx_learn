#include <util/PlutoUtil.h>
#include <sys/SysEnv.h>
#include <util/CUtil.h>

PlutoUtil::PlutoUtil(){}

PlutoUtil::~PlutoUtil(){}

std::string const PlutoUtil::GetAppRoot(){
	return SysEnv::GetVariable("PLU_ROOT");
}

std::string PlutoUtil::GetFullPath(const char* path){
	return SysEnv::GetVariable("PLU_ROOT") + path;
}

std::wstring PlutoUtil::GetFullPathW(const char* path){
	std::string pstr = SysEnv::GetVariable("PLU_ROOT") + path;
	std::wstring wst;
	str_2_wstr(pstr, wst);
	return wst;
}

void PlutoUtil::AddTexture(Mesh* mesh, const char* texturePath){
	DXVisual &visual = mesh->visual;
	std::string path = PlutoUtil::GetFullPath(texturePath);
	visual.PreAddTexture(path);
}