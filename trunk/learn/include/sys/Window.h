#pragma once
#include <sys/BaseApp.h>
#include <graphics/Camera.h>

class Window : public BaseApp
{
public:
	 Window();
	~ Window();

	virtual void Update();
protected:
	void UpdateByRMouse(float value);
	void UpdateByLMouse(float value);

protected:
	Camera _camera;
	Mesh* _crrMesh = nullptr;

	Scene _scene;

private:
	private int _lastMouseX = 0;
	private int _lastMouseY = 0;
};

