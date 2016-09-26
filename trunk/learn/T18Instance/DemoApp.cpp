#include <util/ObjParser.h>
#include "DemoApp.h"

DemoApp::DemoApp(){
}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;

	/*׼�����㻺������*/
	_currMesh = _scene.GetMesh(0);
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	PreSetVSConstBufferSize(_currMesh, sizeof(Float4));
	PreAddLayout(_currMesh);
	PreAddLayoutInstance(_currMesh);

	InitVisual(_currMesh, L"shader/Instance.fx", "assets/t_01.dds");
	_dxEngine.EnableAlphaBlend();
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Update(){
	Window::Update();

	Float4 insPos;
	_dxEngine.UpdateVSSubResource(GetVisual(), 1, &insPos);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}