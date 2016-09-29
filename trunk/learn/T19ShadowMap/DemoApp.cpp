#include <util/ObjParser.h>
#include <graphics/GeoCreater.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_camera.SetPos(0, 1.0f, -4.0);
	_scene.camera = &_camera;

	//平行光坐标在地图左上角, 超右下方观看
	_scene.lightList[0] = new Light();
	_scene.lightList[0]->type = Light::TYPE_DIRECTION;
	_scene.lightList[0]->ambientColor = Color{ 0.0f, 0.0f, 0.0f, 0.3f };
	_scene.lightList[0]->diffuseColor = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[0]->specularColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	_scene.lightList[0]->pos = Vector3D(-4.0f, 4.0f, 0.0f);
	_scene.lightList[0]->dir = Vector3D(1.0f, -1.0f, 0.0f);
	_scene.lightNum = 1;


	//1. 方块, 坐标在0, 0.5, 0
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_currMesh = _scene.GetMesh(0);
	_currMesh->SetWorldPos(0, 0, 0);
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	InitVisual(_currMesh, L"shader/Phong.fx", "assets/t_02.dds");

	//2. 地板, 坐标在0, -0.5f, 0
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(0, 0, 0);
	GeoCreater::CreateFloor(*_currMesh);
	_scene.AddMesh(_currMesh);
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	InitVisual(_currMesh, L"shader/Phong.fx", "assets/t_01.dds");

	_dxEngine.CreateRTT();
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::UpdateConstForPhong(){
	Light light = *_scene.lightList[0];
	Color ac = light.ambientColor;
	Color dc = light.diffuseColor;
	Color sc = light.specularColor;
	Vector3D d = light.dir;
	Vector3D p = light.pos;
	
	PhongConstBuffer pb;
	pb.eyeWorldPos = Float4{ 0.0f, 0.0f, -1.0f, 1.0f };

	DirectionLight dl = DirectionLight{
		Float4A{ ac.r, ac.g, ac.b, ac.a },
		Float4A{ dc.r, dc.g, dc.b, dc.a },
		Float4A{ sc.r, sc.g, sc.b, sc.a },
		Float4{ p.x, p.y, p.z, 1.0f },
		Float4{ d.x, d.y, d.z, 1.0f } };
	pb.directionLight = dl;

	_dxEngine.UpdatePSSubResource(GetVisual(), 0, &pb);
	_dxEngine.UpdateVSSubResource(GetVisual(), 1, &_lightSpaceBuffer);
}

void DemoApp::Update(){
	UpdateByKey(0.002f);
	UpdateByLMouse(0.003f);
	UpdateByRMouse(0.003f);

	_lastMouseX = GetMouseX();
	_lastMouseY = GetMouseY();
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	//1. 调整相机姿态到灯光姿态
	Vector3D oriPos = _camera.position;
	float head = _camera.heading;
	float pitch = _camera.pitch;
	float bank = _camera.bank;

	Light &light = *_scene.lightList[0];
	_camera.SetPos(light.pos);
	_camera.SetAspect(1280, 1280);
	_camera.SetEulerAngle(-1.6f, -0.5f, 0);

	//设置light space的转换数据
	_lightSpaceBuffer.view = _camera.GetWorldToCameraMatrix().transpose();
	_lightSpaceBuffer.perspective = _camera.GetCameraToProjMatrix().transpose();
	_lightSpaceBuffer.pos = Float4(light.pos.x, light.pos.y, light.pos.z, 1.0f);


	//2. 设置mesh的shader为depth, 将场景的深度信息绘制到RTT
	_currMesh = _scene.GetMesh(0);
	_currMesh->visual.Reset();
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	InitVisual(_currMesh, L"shader/Depth.fx");
	_currMesh = _scene.GetMesh(1);
	_currMesh->visual.Reset();
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	InitVisual(_currMesh, L"shader/Depth.fx");

	//RTT
	_dxEngine.UseRTT();
	//更新mvp
	UpdateConstBuff();
	_currMesh = _scene.GetMesh(0);
	_dxEngine.DrawVisual(GetVisual());
	_currMesh = _scene.GetMesh(1);
	_dxEngine.DrawVisual(GetVisual());

	//渲染正常的场景
	//回归相机
	_camera.SetPos(oriPos);
	_camera.SetEulerAngle(head, pitch, bank);
	_camera.SetAspect(_width, _height);

	//设置mesh的shader为正常渲染shader
	_currMesh = _scene.GetMesh(0);
	_currMesh->visual.Reset();
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	PreSetVSConstBufferSize(_currMesh, sizeof(LightSpaceBuffer));
	PreSetPSConstBufferSize(_currMesh, sizeof(PhongConstBuffer));
	InitVisual(_currMesh, L"shader/Phong.fx", "assets/t_02.dds");
	UpdateConstForPhong();
	
	_currMesh = _scene.GetMesh(1);
	_currMesh->visual.Reset();
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	PreSetVSConstBufferSize(_currMesh, sizeof(LightSpaceBuffer));
	PreSetPSConstBufferSize(_currMesh, sizeof(PhongConstBuffer));
	InitVisual(_currMesh, L"shader/Phong.fx", "assets/t_01.dds");
	UpdateConstForPhong();
	UpdateConstBuff();

	//绘制到后缓冲
	_dxEngine.SetDefaultRenderTargetView();
	_dxEngine.ClearBuffers();
	_currMesh = _scene.GetMesh(0);
	_dxEngine.DrawVisualByRTT(GetVisual());
	_currMesh = _scene.GetMesh(1);
	_dxEngine.DrawVisualByRTT(GetVisual());
	_dxEngine.Present();
}