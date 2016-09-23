#include <util/ObjParser.h>
#include "DemoApp.h"

DemoApp::DemoApp(){
	_scroll.scroll = 0;
	_fade.scroll = 0;
	_tag = 0.0001f;
}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;

	/*准备顶点缓冲数据*/
	_currMesh = _scene.GetMesh(0);
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	PreSetVSConstBufferSize(_currMesh, sizeof(ScrollBuffer));
	PreSetVSConstBufferSize(_currMesh, sizeof(ScrollBuffer));
	InitVisual(_currMesh, L"shader/scroll_uv.fx", "assets/t_01.dds");
	_dxEngine.EnableAlphaBlend();
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Update(){
	Window::Update();

	_scroll.scroll += 0.0001f;
	_dxEngine.UpdateVSSubResource(GetVisual(), 1, &_scroll);
	if(_fade.scroll > 1.0f || _fade.scroll < 0.0f)_tag *= -1.0f;

	_fade.scroll += _tag;
	_dxEngine.UpdateVSSubResource(GetVisual(), 2, &_fade);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}
