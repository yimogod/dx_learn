#pragma once
#include <BaseDataStruct.h>
#include <sys/BaseApp.h>
#include <scene/Camera.h>
#include <graphics/DXVisual.h>

class Window : public BaseApp{
public:
	 Window();
	~ Window();

	virtual bool Init(HINSTANCE const &ins, HWND const &hwnd);
	virtual void Update();
	virtual void Destroy();
protected:
	virtual bool LoadContent() = 0;
	virtual void UnloadContent() = 0;
	virtual void PreAddOtherConstBuffer(DXVisual &visual);//预添加其他constbuffer, 比如灯光之类的

	void UpdateByRMouse(float value);
	void UpdateByLMouse(float value);
	void UpdateByKey(float value);
	void UpdateConstBuff();

	void InitVisual(Mesh* mesh, wchar_t* vsName);
	void InitVisual(Mesh* mesh, wchar_t* vsName, const char* texturePath);
	void AddTexture(Mesh* mesh, const char* texturePath);//加载多张图片

	inline DXVisual& GetVisual() const;
protected:
	Mesh* _currMesh = nullptr;

	Camera _camera;
	Scene _scene;

	DXEngine _dxEngine;

private:
	int _lastMouseX = 0;
	int _lastMouseY = 0;
};

inline DXVisual& Window::GetVisual() const{
	return _currMesh->visual;
}