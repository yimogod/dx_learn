#include <util/ObjParser.h>
#include "DemoApp.h"
#include <util/PlutoUtil.h>

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;

	Transform* trans = nullptr;
	ObjParser reader;
	Mesh* mesh = nullptr;
	Material* mat = nullptr;

	mat = new Material("shader/Triangle.fx");
	mat->AddTexture("assets/t_01.dds");
	mat->Set_VS_CBufferSize(sizeof(MVPConstBuffer));

	mesh = reader.Read(PlutoUtil::GetFullPath("assets/cube.obj").c_str());
	mesh->material = std::shared_ptr<Material>(mat);

	trans = new Transform();
	trans->SetWorldPos(0, 0.5f, 1.0f);
	trans->SetMesh(mesh);
	/*准备顶点缓冲数据*/
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