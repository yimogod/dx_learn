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

	/*准备顶点缓冲数据*/
	_currMesh = _scene.GetMesh(0);
	PreAddLayout(_currMesh);
	PreAddLayoutInstance(_currMesh);
	AddTexture(_currMesh, "assets/t_01.dds");

	Vertex insList[4];
	insList[0].pos = Float4(-1.5f, -1.5f, 5.0f, 1.0f);
	insList[0].pos = Float4(-1.5f, 1.5f, 5.0f, 1.0f);
	insList[0].pos = Float4(1.5f, -1.5f, 5.0f, 1.0f);
	insList[0].pos = Float4(1.5f, 1.5f, 5.0f, 1.0f);

	InitVisual(_currMesh, insList, 4, L"shader/Instance.fx");
	_dxEngine.EnableAlphaBlend();
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}
