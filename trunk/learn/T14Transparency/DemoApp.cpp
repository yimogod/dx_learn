#include <graphics/GeoCreater.h>
#include "DemoApp.h"
//有个现象需要注意一下. 如果开启blend, 那么得先绘制不透明物体
//否则, 透明物体会对空背景进行blend, 然后再绘制不透明物体的话, 就达不到alpha效果
//这意味着, 场景管理需要对透明物体进行排序,
//先绘制不透明, 再绘制透明, 然后接着绘制更近的透明或者不透明
DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_dxEngine.EnableAlphaBlend();
	
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(0, 0, 0.0f);
	GeoCreater::CreateSprite(*_currMesh);
	_currMesh->SetColor(Color(1.0f, 1.0f, 1.0f, 0.6f));
	_scene.AddMesh(_currMesh);
	InitVisual(_currMesh, L"shader/blending.fx", "assets/t_01.dds");

	//这个更远些
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(1.0f, 0, 2.0f);
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);
	InitVisual(_currMesh, L"shader/blending.fx", "assets/t_02.dds");

	//这个靠左些
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(-0.6f, 0, 2.2f);
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);
	InitVisual(_currMesh, L"shader/blending.fx", "assets/t_01.dds");
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	//注意, 绘制顺序, 是先绘制不透明, 再绘制透明
	_dxEngine.ClearBuffers();
	_currMesh = _scene.meshList[2];
	_dxEngine.DrawVisual(GetVisual());
	_currMesh = _scene.meshList[1];
	_dxEngine.DrawVisual(GetVisual());

	_currMesh = _scene.meshList[0];
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}