#pragma once
#include <string>

#include <sys/SysEnv.h>
#include <scene/Scene.h>
#include <graphics/DXEngine.h>

class BaseApp{
public:
	BaseApp();
	virtual ~BaseApp();

	virtual bool Init(HINSTANCE const &ins, HWND const &hwnd);
	virtual void Destroy() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;

	void OnMove(int screenX, int screenY);
	void OnKeyDown(int key, int screenX, int screenY);
	void OnKeyUp(int key, int screenX, int screenY);
	void OnMouseDown(int screenX, int screenY);
	void OnMouseUp(int screenX, int screenY);
	void OnRMouseDown(int screenX, int screenY);
	void OnRMouseUp(int screenX, int screenY);

protected:
	inline bool isKeyDown(int keycode) const;
	inline bool isLMouseDown() const;
	inline bool isRMouseDown() const;
	inline int GetMouseX() const;
	inline int GetMouseY() const;

	std::string const GetAppRoot();
	std::string GetFullPath(char* path);
	std::wstring GetFullPathW(char* path);

protected:
	HINSTANCE _ins;
	HWND _hwnd;

	int _width = 0; //屏幕宽度
	int _height = 0; //屏幕高度

	SysEnv _env;

private:
	int _pressedKey = -1; //按下的按钮
	int _mouseX = -1; //鼠标的位置
	int _mouseY = -1; //鼠标的位置
	bool _isLMouseDown = false;
	bool _isRMouseDown = false;
};

inline bool BaseApp::isKeyDown(int keycode) const{
	return keycode == _pressedKey;
}

inline bool BaseApp::isLMouseDown() const{
	return _isLMouseDown;
}

inline bool BaseApp::isRMouseDown() const{
	return _isRMouseDown;
}

inline int BaseApp::GetMouseX() const{
	return _mouseX;
}

inline int BaseApp::GetMouseY() const{
	return _mouseY;
}
