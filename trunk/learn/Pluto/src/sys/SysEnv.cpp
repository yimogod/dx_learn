#include <sys/SysEnv.h>

SysEnv::SysEnv(){}
SysEnv::~SysEnv(){}

std::string SysEnv::GetVariable(std::string const& name){
	size_t size;
	getenv_s(&size, nullptr, 0, name.c_str());
	if(size > 0){
		char* tmpvar = new char[size];
		errno_t result = getenv_s(&size, tmpvar, size, name.c_str());
		std::string var = (result == 0 ? std::string(tmpvar) : "");
		delete[] tmpvar;
		return var;
	}
	
	return "";
}
