#pragma once
#include <string>

#include <sys/SysEnv.h>
#include <graphics/Scene.h>

class BaseApp{
public:
	BaseApp();
	virtual ~BaseApp();

	bool init(HINSTANCE ins, HWND hwnd);
	void destroy();

	virtual bool loadContent();
	virtual void unloadContent();

	virtual void render();
	virtual void update();

protected:
	HINSTANCE _ins;
	HWND _hwnd;

	int _width, _height;
protected:
	Scene _scene;
	SysEnv _env;

	std::string getAppRoot();
	std::string getFullPath(char* path);
	std::wstring getFullPathW(char* path);
};