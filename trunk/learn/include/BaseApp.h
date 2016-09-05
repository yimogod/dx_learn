#pragma once
#include <string>

#include <sys/SysEnv.h>
#include <graphics/Scene.h>
#include <graphics/DXEngine.h>

class BaseApp{
public:
	BaseApp();
	virtual ~BaseApp();

	bool Init(HINSTANCE const &ins, HWND const &hwnd);
	void Destroy();

	virtual bool LoadContent() = 0;
	virtual void UnloadContent() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	HINSTANCE _ins;
	HWND _hwnd;

	int _width, _height;

	DXEngine _dxEngine;

protected:
	Scene _scene;
	SysEnv _env;

	std::string const GetAppRoot();
	std::string GetFullPath(char* path);
	std::wstring GetFullPathW(char* path);
};