#include <util/ObjParser.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;
	
	/*准备顶点缓冲数据*/
	_currMesh = _scene.getMesh(0);
	Vertex* vertices = new Vertex[_currMesh->indexNum];
	_currMesh->getVertexList(vertices);

	InitVisual(_visual, L"shader/Triangle.fx", vertices);
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

	_dxEngine.GetContext()->Draw(_currMesh->indexNum, 0);
	_dxEngine.Present();
}