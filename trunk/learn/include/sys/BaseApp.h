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
	inline bool isKeyDown(int keycode) const;

	std::string const GetAppRoot();
	std::string GetFullPath(char* path);
	std::wstring GetFullPathW(char* path);

private:
	void OnMove(int screenX, int screenY);
	void OnKeyDown(int key, int screenX, int screenY);
	void OnKeyUp(int key, int screenX, int screenY);
	void OnMouseDown(int screenX, int screenY);
	void OnMouseUp(int screenX, int screenY);
	void OnRMouseDown(int screenX, int screenY);
	void OnRMouseUp(int screenX, int screenY);

protected:
	HINSTANCE _ins;
	HWND _hwnd;

	int _width = 0; //屏幕宽度
	int _height = 0; //屏幕高度
	int _mouseX = -1; //鼠标的位置
	int _mouseY = -1; //鼠标的位置
	
	bool _isLMouseDown = false;
	bool _isRMouseDown = false;

	SysEnv _env;

private:
	int _pressedKey = -1; //按下的按钮

};

inline bool BaseApp::isKeyDown(int keycode) const{
	return keycode == _pressedKey;
}