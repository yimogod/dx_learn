#pragma once
#include <string>

#include <math/algebra/Matrix4x4.h>
#include <graphics/Vertex.h>
#include <sys/SysEnv.h>
#include <graphics/Scene.h>

using namespace std;

struct ConstantBuffer{
	Matrix4x4 model;
	Matrix4x4 view;
	Matrix4x4 perspective;
};

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
	void initDevice();
	void initDevice_v2();
protected:
	bool isKeyDown(char keycode);
	bool isLMouseDown();
	bool isRMouseDown();
	void UpdatePosByRMouse(Camera* camera, float value);
	void UpdatePosByLMouse(Mesh* mesh, float value);
	
	float _mouseX, _mouseY;
	bool _isRMouseDown = false;
	bool _isLMouseDown = false;

protected:
	Scene _scene;
	SysEnv _env;

	string getAppRoot();
	string getFullPath(char* path);
	wstring getFullPathW(char* path);
};