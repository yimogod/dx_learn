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

	void InitVisual(Mesh* mesh, wchar_t* vsName);
	void InitVisual(Mesh* mesh, wchar_t* vsName, const char* texturePath);
	void AddTexture(Mesh* mesh, const char* texturePath);//���ض���ͼƬ
	inline void AddSamplerState(D3D11_TEXTURE_ADDRESS_MODE adress);//�������sampler

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

inline void Window::AddSamplerState(D3D11_TEXTURE_ADDRESS_MODE adress){
	_currMesh->visual.PreAddSampleState(adress);
}