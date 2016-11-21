#pragma once
#include <string>

class SysEnv{
public:
	SysEnv();
	~SysEnv();

	 static std::string GetVariable(std::string const& name);
};