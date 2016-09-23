#include <util/ObjParser.h>
#include "DemoApp.h"

DemoApp::DemoApp(){
	_scroll.scroll = 0;
	//xz平面下面全部剪切!
	_clipBuffer.x = 0.0f;
	_clipBuffer.y = 1.0f;
	_clipBuffer.z = 0.0f;
	_clipBuffer.w = 0.0f;
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
	PreSetVSConstBufferSize(_currMesh, sizeof(Float4));
	InitVisual(_currMesh, L"shader/clip_plane.fx", "assets/t_01.dds");
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Update(){
	Window::Update();

	_scroll.scroll += 0.0001f;
	_dxEngine.UpdateVSSubResource(GetVisual(), 1, &_scroll);
	_dxEngine.UpdateVSSubResource(GetVisual(), 2, &_clipBuffer);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}
