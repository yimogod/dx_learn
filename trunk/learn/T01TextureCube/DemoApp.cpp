#include <util/ObjParser.h>
#include "DemoApp.h"
#include <util/PlutoUtil.h>

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.Read(PlutoUtil::GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;
	
	Material* mat = new Material("shader/Triangle.fx");
	mat->AddTexture("assets/t_01.dds");
	mat->SetVSConstBufferSize(sizeof(MVPConstBuffer));

	/*准备顶点缓冲数据*/
	_currMesh = _scene.GetMesh(0);
	_currMesh->SetWorldPos(0, 0.5f, 1.0f);
	_currMesh->material = mat;

	_currMesh->Init();
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}