#pragma once
#include <windows.h>
#include <Main.h>
#include <sys/Window.h>

//scroll = 3��ͼƬ��scroll�ٶ�. ������3��noiseͼƬ, ���е�2,3���Ǹ��ݵ�һ����ƽ�����ɵ�
//scale = 3��ͼƬ��scroll�ٶ�. ������3��noiseͼƬ
struct NoiseBuffer{
	float frame;
	Float3 scroll; 
	Float3 scale;
	float pad0;
};

//Ť��
//����noiseͼƬ��Ť��
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
