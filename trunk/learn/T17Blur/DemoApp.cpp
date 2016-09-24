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
	_currMesh->SetWorldPos(0, -0.5f, 0);
	InitVisual(_currMesh, L"shader/Triangle.fx", "assets/t_01.dds");
	
	_currMesh = new Mesh();
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);
	/*准备顶点缓冲数据*/
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	PreSetVSConstBufferSize(_currMesh, sizeof(Float4)); 
	InitVisual(_currMesh, L"shader/Blur.fx");
	
	_currMesh = new Mesh();
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);
	InitVisual(_currMesh, L"shader/Screen.fx");

	//创建2个rtt, 第一个用于渲染cube, 第二个用于blur, 然后渲染到后缓冲
	_dxEngine.CreateRTT(_width, _height); 
	int halfWidth = (int)(0.5f * _width);
	int halfHeight = (int)(0.5f * _height);
	_dxEngine.CreateRTT(halfWidth, halfHeight);
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Update(){
	Window::Update();

	_currMesh = _scene.GetMesh(1);
	Float4 screenSize;
	screenSize.x = (_width * 0.5f);
	screenSize.y = (_height * 0.5f);
	screenSize.z = 0;
	screenSize.w = 1.0f;
	_dxEngine.UpdateVSSubResource(GetVisual(), 1, &screenSize);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;
	//render to texture
	_currMesh = _scene.GetMesh(0);
	_dxEngine.UseRTT(0);
	_dxEngine.DrawVisual(GetVisual());

	//blur rtt
	_currMesh = _scene.GetMesh(1);
	_dxEngine.UseRTT(1);
	_dxEngine.DrawVisualByRTT(GetVisual(), 0);
	
	//render to scene
	_currMesh = _scene.GetMesh(2);
	_dxEngine.SetDefaultRenderTargetView();
	_dxEngine.ClearBuffers();
	_dxEngine.TurnOffZBuffer();
	_dxEngine.DrawVisualByRTT(GetVisual(), 1);
	
	_dxEngine.Present();
}
