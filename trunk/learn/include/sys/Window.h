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
	void InitVisual(Mesh* mesh, char* vertAddOn, int vertAddOnNum, wchar_t* vsName);
	void InitVisual(Mesh* mesh, wchar_t* vsName, const char* texturePath);

	//方法代理, 为了让代码更好看
	void AddTexture(Mesh* mesh, const char* texturePath);//加载多张图片
	inline void AddSamplerState(Mesh* mesh, D3D11_TEXTURE_ADDRESS_MODE adress);//创建多个sampler
	inline void PreSetVSConstBufferSize(Mesh* mesh, int byteWidth);
	inline void PreSetPSConstBufferSize(Mesh* mesh, int byteWidth);
	inline void PreAddDefaultLayout(Mesh* mesh);
	inline void PreAddLayoutInstance(Mesh* mesh);

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

inline void Window::AddSamplerState(Mesh* mesh, D3D11_TEXTURE_ADDRESS_MODE adress){
	mesh->visual.PreAddSampleState(adress);
}

inline void Window::PreSetVSConstBufferSize(Mesh* mesh, int byteWidth){
	mesh->visual.PreSetVSConstBufferSize(byteWidth);
}

inline void Window::PreSetPSConstBufferSize(Mesh* mesh, int byteWidth){
	mesh->visual.PreSetPSConstBufferSize(byteWidth);
}

inline void Window::PreAddDefaultLayout(Mesh* mesh){
	mesh->visual.PreAddDefaultLayout();
}

inline void Window::PreAddLayoutInstance(Mesh* mesh){
	mesh->visual.PreAddDefaultLayoutInstance();
}