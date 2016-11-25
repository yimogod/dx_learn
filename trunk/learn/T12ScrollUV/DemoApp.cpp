#include "DemoApp.h"
#include <util/PlutoUtil.h>

DemoApp::DemoApp(){
	_scroll.scroll = 0;
	_fade.scroll = 0;
	_tag = 0.0001f;
}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_scene.renderType = Scene::RENDER_TYPE_FRAME;

	Transform* trans = nullptr;
	Mesh* mesh = nullptr;
	Material* mat = nullptr;

	mat = new Material("shader/scroll_uv.fx");
	mat->AddTexture("assets/t_01.dds");
	mat->Set_VS_CBufferSize(sizeof(MVPConstBuffer));
	mat->Set_VS_CBufferSize(sizeof(ScrollBuffer));
	mat->Set_VS_CBufferSize(sizeof(ScrollBuffer));

	mesh = new Mesh(PlutoUtil::GetFullPath("assets/cube.obj"));
	mesh->material = std::shared_ptr<Material>(mat);

	trans = new Transform();
	trans->SetWorldPos(0, 0.5f, 1.0f);
	trans->SetMesh(mesh);

	_scene.AddTransform(trans);
	_currTrans = trans;

	DXEngine::Instance().EnableAlphaBlend();
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Update(){
	_scroll.scroll += 0.0001f;
	if(_fade.scroll > 1.0f || _fade.scroll < 0.0f)_tag *= -1.0f;
	_fade.scroll += _tag;

	_scene.GetMaterial(0)->Set_VS_CBufferData(1, (char *)&_scroll);
	_scene.GetMaterial(0)->Set_VS_CBufferData(2, (char *)&_fade);
	
	Window::Update();
}

void DemoApp::Render(){
	if(!DXEngine::Instance().GetReady())return;

	DXEngine::Instance().ClearBuffers();
	_scene.Draw();
	DXEngine::Instance().Present();
}