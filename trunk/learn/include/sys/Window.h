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
	virtual void PreAddOtherConstBuffer(DXVisual &visual);//Ԥ�������constbuffer, ����ƹ�֮���

	void UpdateByRMouse(float value);
	void UpdateByLMouse(float value);
	void UpdateByKey(float value);
	void UpdateConstBuff();

	void InitVisual(DXVisual &visual, Mesh* mesh, wchar_t* vsName);
	void InitVisual(DXVisual &visual, Mesh* mesh, wchar_t* vsName, const char* texturePath);
protected:
	Mesh* _currMesh = nullptr;

	Camera _camera;
	Scene _scene;
	DXVisual _visual;

	DXEngine _dxEngine;

private:
	int _lastMouseX = 0;
	int _lastMouseY = 0;
};