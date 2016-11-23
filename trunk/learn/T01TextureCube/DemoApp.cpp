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
	std::shared_ptr<Mesh> mesh = nullptr;
	std::shared_ptr<Material> mat = nullptr;

	mat = std::shared_ptr<Material>(new Material("shader/Triangle.fx"));
	mat->AddTexture("assets/t_01.dds");
	mat->SetVSConstBufferSize(sizeof(MVPConstBuffer));

	mesh = reader.Read(PlutoUtil::GetFullPath("assets/cube.obj").c_str());
	mesh->material = mat;

	trans = new Transform();
	trans->SetWorldPos(0, 0.5f, 1.0f);
	trans->SetMesh(mesh.get());
	/*准备顶点缓冲数据*/
	_scene.AddTransform(*trans);
	
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