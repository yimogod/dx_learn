#include <util/ObjParser.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;

	_scene.lightList[0] = new Light();
	_scene.lightList[0]->type = Light::TYPE_DIRECTION;
	_scene.lightList[0]->ambientColor = Color{ 0.0f, 0.0f, 0.0f, 0.3f };
	_scene.lightList[0]->diffuseColor = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[0]->specularColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	_scene.lightList[0]->pos = Vector3D(0.0f, 0.0f, 0.0f);
	_scene.lightList[0]->dir = Vector3D(1.0f, -1.0f, 1.0f);
	
	_scene.lightList[1] = new Light();
	_scene.lightList[1]->type = Light::TYPE_POINT;
	_scene.lightList[1]->ambientColor = Color{ 0.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[1]->diffuseColor = Color{ 1.0f, 0.0f, 1.0f, 1.0f };
	_scene.lightList[1]->specularColor = Color{ 0.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[1]->pos = Vector3D(2.0f, 0.0f, 0.0f);
	_scene.lightList[1]->range = 200.0f;
	_scene.lightList[1]->kc = 0.0f;
	_scene.lightList[1]->kl = 1.0f;
	_scene.lightList[1]->kq = 0.0f;

	_scene.lightNum = 2;

	/*准备顶点缓冲数据*/
	_currMesh = _scene.GetMesh(0);
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	PreSetPSConstBufferSize(_currMesh, sizeof(PhongConstBuffer));
	InitVisual(_currMesh, L"shader/Phong.fx", "assets/t_02.dds");
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Update(){
	Window::Update();

	Light* light = _scene.lightList[0];
	Color ac = light->ambientColor;
	Color dc = light->diffuseColor;
	Color sc = light->specularColor;
	Vector3D d = light->dir;

	PhongConstBuffer pb;
	pb.eyeWorldPos = Float4{ 0.0f, 0.0f, -1.0f, 1.0f };

	DirectionLight dl = DirectionLight{
		Float4A{ ac.r, ac.g, ac.b, ac.a },
		Float4A{ dc.r, dc.g, dc.b, dc.a },
		Float4A{ sc.r, sc.g, sc.b, sc.a },
		Float4{ d.x, d.y, d.z, 1.0f } };
	pb.directionLight = dl;

	light = _scene.lightList[1];
	ac = light->ambientColor;
	dc = light->diffuseColor;
	sc = light->specularColor;
	d = light->pos;
	PointLight pl = PointLight{
		Float4A{ ac.r, ac.g, ac.b, ac.a },
		Float4A{ dc.r, dc.g, dc.b, dc.a },
		Float4A{ sc.r, sc.g, sc.b, sc.a },
		Float4{ d.x, d.y, d.z, 1.0f },
		Float3{ light->kc, light->kl, light->kq },
		light->range};
	pb.pointLight = pl;

	_dxEngine.UpdatePSSubResource(GetVisual(), 0, &pb);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;
	
	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());

	_dxEngine.Present();
}