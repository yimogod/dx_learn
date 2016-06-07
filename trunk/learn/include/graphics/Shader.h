#pragma once
class Shader
{
public:
	Shader();
	~Shader();

	wchar_t* fileName;
	char* entryPoint;
	char* shaderModel;
};
