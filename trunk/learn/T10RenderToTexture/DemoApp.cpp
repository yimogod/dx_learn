#include <util/ObjParser.h>
#include <graphics/GeoCreater.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;

	_currMesh = _scene.GetMesh(0);
	InitVisual(_currMesh, L"shader/Triangle.fx", "assets/t_01.dds");

	_currMesh = new Mesh();
	_currMesh->SetWorldPos(0, 0, 0.0f);
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);
	InitVisual(_currMesh, L"shader/Triangle.fx");

	_dxEngine.CreateRTT();
	return true;
}

void DemoApp::Update(){
	Window::Update();
}

void DemoApp::UnloadContent(){}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;
	//render to texture
	_currMesh = _scene.GetMesh(0);
	_dxEngine.UseRTT();
	_dxEngine.DrawVisual(GetVisual());
	
	_currMesh = _scene.GetMesh(1);
	_dxEngine.SetDefaultRenderTargetView();
	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisualByRTT(GetVisual());
	_dxEngine.Present();
}
