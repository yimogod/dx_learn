#include <graphics/GeoCreater.h>
#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_currMesh = new Mesh();
	_currMesh->setWorldPos(0, 0, 0.0f);
	GeoCreater::CreateSprite(*_currMesh);

	/*准备顶点缓冲数据*/
	Vertex* vertices = new Vertex[_currMesh->indexNum];
	_currMesh->getVertexList(vertices);

	InitVisual(_visual, L"shader/sprite.fx", vertices);
	_dxEngine.CreateTexture(GetFullPathW("assets/t_01.dds").c_str());

	delete(vertices);
	return true;
}

void DemoApp::UnloadContent(){
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(_visual);

	_dxEngine.PSSetShaderResources(0);
	_dxEngine.PSSetSamplers(0, 1);
	
	_dxEngine.GetContext()->Draw(_currMesh->indexNum, 0);
	_dxEngine.Present();
}