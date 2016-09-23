#include <graphics/GeoCreater.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(0, 0, 0.0f);
	GeoCreater::CreateFloor(*_currMesh);
	_scene.AddMesh(_currMesh);
	_camera.setPos(0, 0.5f, -3.0f);

	/*准备顶点缓冲数据*/
	InitVisual(_currMesh, L"shader/depth_field.fx");
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}