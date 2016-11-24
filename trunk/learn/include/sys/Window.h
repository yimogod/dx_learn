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

	void UpdateByRMouse(float value);
	void UpdateByLMouse(float value);
	void UpdateByKey(float value);
	void UpdateConstBuff();

	//方法代理, 为了让代码更好看
	inline void AddSamplerState(Mesh* mesh, D3D11_TEXTURE_ADDRESS_MODE adress);//创建多个sampler
protected:
	Transform* _currTrans = nullptr;

	Camera _camera;
	Scene _scene;

	int _lastMouseX = 0;
	int _lastMouseY = 0;
};

inline void Window::AddSamplerState(Mesh* mesh, D3D11_TEXTURE_ADDRESS_MODE adress){
	mesh->visual.PreAddSampleState(adress);
}
