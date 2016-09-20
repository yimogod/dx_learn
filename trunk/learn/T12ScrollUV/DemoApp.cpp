#include <util/ObjParser.h>
#include "DemoApp.h"

DemoApp::DemoApp(){
	_scroll.scroll = 0;
}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;

	/*׼�����㻺������*/
	_currMesh = _scene.GetMesh(0);
	InitVisual(_currMesh, L"shader/scroll_uv.fx", "assets/t_01.dds");
	return true;
}

void DemoApp::PreAddOtherConstBuffer(DXVisual &visual){
	visual.PreSetPSConstBufferSize(sizeof(ScrollBuffer));
}

void DemoApp::UnloadContent(){}

void DemoApp::Update(){
	Window::Update();

	_scroll.scroll += 0.0001f;
	_dxEngine.UpdatePSSubResource(GetVisual(), 0, &_scroll);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}
