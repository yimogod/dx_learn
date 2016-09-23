#include <util/ObjParser.h>
#include <graphics/GeoCreater.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;
	
	/*准备顶点缓冲数据*/
	_currMesh = _scene.GetMesh(0);
	InitVisual(_currMesh, L"shader/Triangle.fx", "assets/t_01.dds");
	
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(0, 0, 0.0f);
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);
	/*准备顶点缓冲数据*/
	InitVisual(_currMesh, L"shader/Blur.fx");

	
	int halfWidth = (int)(0.5f * _width);
	int halfHeight = (int)(0.5f * _height);
	_dxEngine.CreateRTT(halfWidth, halfHeight);
	return true;
}

void DemoApp::PreAddOtherConstBuffer(DXVisual &visual){
	visual.PreSetVSConstBufferSize(sizeof(Float4));
}

void DemoApp::UnloadContent(){}

void DemoApp::Update(){
	Window::Update();

	Float4 screenSize;
	screenSize.x = _width;
	screenSize.y = _height;
	screenSize.z = 0;
	screenSize.w = 1.0f;

	_dxEngine.UpdateVSSubResource(GetVisual(), 1, &screenSize);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;
	//render to texture
	_currMesh = _scene.GetMesh(0);
	_dxEngine.UseRTT();
	_dxEngine.DrawVisual(GetVisual());

	//blur rtt
	_currMesh = _scene.GetMesh(1);
	_dxEngine.SetDefaultRenderTargetView();
	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual2RTT(GetVisual());
	_dxEngine.Present();
}
