#include <util/ObjParser.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;
	
	/*准备顶点缓冲数据*/
	_currMesh = _scene.GetMesh(0);
	InitVisual(_currMesh, L"shader/Triangle.fx", "assets/t_01.dds");
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}