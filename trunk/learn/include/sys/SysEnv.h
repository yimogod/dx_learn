#pragma once
#include <string>

class SysEnv{
public:
	SysEnv();
	~SysEnv();

	std::string getVariable(std::string const& name);
};