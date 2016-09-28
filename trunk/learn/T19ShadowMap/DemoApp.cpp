#include <util/ObjParser.h>
#include <graphics/GeoCreater.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;

	//平行光坐标在地图左上角, 超右下方观看
	_scene.lightList[0] = new Light();
	_scene.lightList[0]->type = Light::TYPE_DIRECTION;
	_scene.lightList[0]->ambientColor = Color{ 0.0f, 0.0f, 0.0f, 0.3f };
	_scene.lightList[0]->diffuseColor = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[0]->specularColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	_scene.lightList[0]->pos = Vector3D(-1.0f, 1.0f, 0.0f);
	_scene.lightList[0]->dir = Vector3D(1.0f, -1.0f, 1.0f);
	_scene.lightNum = 1;


	//1. 方块, 坐标在0, 0.5, 0
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_currMesh = _scene.GetMesh(0);
	_currMesh->SetWorldPos(0, 0.5f, 0);
	InitVisual(_currMesh, L"shader/Phong.fx", "assets/t_02.dds");

	//2. 地板, 坐标在0, -0.5f, 0
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(0, -0.5f, 0);
	GeoCreater::CreateFloor(*_currMesh);
	_scene.AddMesh(_currMesh);
	InitVisual(_currMesh, L"shader/Triangle.fx");


	_dxEngine.CreateRTT();
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::UpdateConstForPhong(){
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
	
	//RTT
	_dxEngine.UseRTT();
	//1. 调整相机姿态到灯光姿态
	Light &light = *_scene.lightList[0];
	_camera.SetPos(light.pos);
	_camera.SetEulerAngle(0, 0, 45.0f);

	//2. 设置mesh的shader为depth, 将场景的深度信息绘制到RTT
	_currMesh = _scene.GetMesh(0);
	_currMesh->visual.Reset();
	InitVisual(_currMesh, L"shader/Depth.fx");
	_currMesh = _scene.GetMesh(1);
	_currMesh->visual.Reset();
	InitVisual(_currMesh, L"shader/Depth.fx");



	//渲染正常的场景
	//回归相机
	_camera.SetPos(0, 0, -2.0f);
	_camera.SetEulerAngle(0, 0, 0);
	//设置mesh的shader为正常渲染shader
	_currMesh = _scene.GetMesh(0);
	_currMesh->visual.Reset();
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	InitVisual(_currMesh, L"shader/Phong.fx");
	_currMesh = _scene.GetMesh(1);
	_currMesh->visual.Reset();
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	PreSetPSConstBufferSize(_currMesh, sizeof(PhongConstBuffer));
	InitVisual(_currMesh, L"shader/Phong.fx");
	UpdateConstForPhong();


	//绘制到后缓冲
	_dxEngine.SetDefaultRenderTargetView();
	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());

	_dxEngine.Present();
}