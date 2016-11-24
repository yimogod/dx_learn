#include "DemoApp.h"
#include <util/PlutoUtil.h>

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_scene.renderType = Scene::RENDER_TYPE_FRAME;

	Transform* trans = nullptr;
	Mesh* mesh = nullptr;
	Material* mat = nullptr;

	mat = new Material("shader/Triangle.fx");
	mat->AddTexture("assets/t_01.dds");
	mat->Set_VS_CBufferSize(sizeof(MVPConstBuffer));

	mesh = new Mesh(PlutoUtil::GetFullPath("assets/cube.obj"));
	mesh->material = std::shared_ptr<Material>(mat);

	trans = new Transform();
	trans->SetWorldPos(0, 0.5f, 1.0f);
	trans->SetMesh(mesh);

	_scene.AddTransform(trans);
	_currTrans = trans;
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Render(){
	if(!DXEngine::Instance().GetReady())return;

	DXEngine::Instance().ClearBuffers();
	_scene.Draw();
	DXEngine::Instance().Present();
}