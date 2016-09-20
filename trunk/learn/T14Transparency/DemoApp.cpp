#include <graphics/GeoCreater.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(0, 0, 0.0f);
	GeoCreater::CreateSprite(*_currMesh);
	_currMesh->SetColor(Color(1.0f, 1.0f, 1.0f, 0.2f));
	_scene.AddMesh(_currMesh);
	InitVisual(_scene.meshList[0], L"shader/blending.fx", "assets/t_01.dds");

	//这个更远些
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(1.5f, 0, 2.0f);
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);

	/*准备顶点缓冲数据*/
	InitVisual(_scene.meshList[1], L"shader/blending.fx", "assets/t_02.dds");
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.EnableAlphaBlend();
	_currMesh = _scene.meshList[0];
	_dxEngine.DrawVisual(GetVisual());

	_currMesh = _scene.meshList[1];
	_dxEngine.DrawVisual(GetVisual());
	//disableAlphaBlend();
	_dxEngine.Present();
}