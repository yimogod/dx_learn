#pragma once
#include <sys/BaseApp.h>
#include <graphics/Camera.h>

class Window : public BaseApp
{
public:
	 Window();
	~ Window();

	//virtual bool LoadContent() = 0;
	//virtual void UnloadContent() = 0;

	virtual void Update();
	//virtual void Render() = 0;
protected:
	void UpdateByRMouse(float value);
	void UpdateByLMouse(float value);

protected:
	Camera _camera;
	Mesh* _crrMesh = nullptr;

	Scene _scene;
	DXEngine _dxEngine;

private:

};

