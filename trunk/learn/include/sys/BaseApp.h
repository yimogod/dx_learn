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

	int _width = 0; //��Ļ���
	int _height = 0; //��Ļ�߶�
	int _mouseX = -1; //����λ��
	int _mouseY = -1; //����λ��
	
	bool _isLMouseDown = false;
	bool _isRMouseDown = false;

	SysEnv _env;

private:
	int _pressedKey = -1; //���µİ�ť

};

inline bool BaseApp::isKeyDown(int keycode) const{
	return keycode == _pressedKey;
}