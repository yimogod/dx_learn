#pragma once
#include <windows.h>
#include <Main.h>
#include <sys/Window.h>

//scroll = 3张图片的scroll速度. 我们有3张noise图片, 其中第2,3张是根据第一张坐平铺生成的
//scale = 3张图片的scroll速度. 我们有3张noise图片
struct NoiseBuffer{
	float frame;
	Float3 scroll; 
	Float3 scale;
	float pad0;
};

//扭曲
//三张noise图片的扭曲
struct DistortionBuffer{
	Float2 distortion1;
	Float2 distortion2;
	Float2 distortion3;
	float distortionScale;
	float distortionBias;
};

class DemoApp : public Window{
public:
	DemoApp();
	virtual ~DemoApp();

	virtual bool LoadContent();
	virtual void PreAddOtherConstBuffer(DXVisual &visual);
	virtual void UnloadContent();

	virtual void Update();
	virtual void Render();

private:
	NoiseBuffer _noise;
	DistortionBuffer _distortion;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow){
	MainInfo info;
	info.app = new DemoApp();
	info.icon = nullptr;
	info.title = L"Scroll UV";
	info.width = 1200;
	info.height = 960;

	return MainBody(hInstance, hPrevInstance, lpCmdLine, nCmdShow, info);
}
