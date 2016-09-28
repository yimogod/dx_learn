#include <util/ObjParser.h>
#include <graphics/GeoCreater.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;

	//ƽ�й������ڵ�ͼ���Ͻ�, �����·��ۿ�
	_scene.lightList[0] = new Light();
	_scene.lightList[0]->type = Light::TYPE_DIRECTION;
	_scene.lightList[0]->ambientColor = Color{ 0.0f, 0.0f, 0.0f, 0.3f };
	_scene.lightList[0]->diffuseColor = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[0]->specularColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	_scene.lightList[0]->pos = Vector3D(-1.0f, 1.0f, 0.0f);
	_scene.lightList[0]->dir = Vector3D(1.0f, -1.0f, 1.0f);
	_scene.lightNum = 1;


	//1. ����, ������0, 0.5, 0
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_currMesh = _scene.GetMesh(0);
	_currMesh->SetWorldPos(0, 0.5f, 0);
	InitVisual(_currMesh, L"shader/Phong.fx", "assets/t_02.dds");

	//2. �ذ�, ������0, -0.5f, 0
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
	//1. ���������̬���ƹ���̬
	Light &light = *_scene.lightList[0];
	_camera.SetPos(light.pos);
	_camera.SetEulerAngle(0, 0, 45.0f);

	//2. ����mesh��shaderΪdepth, �������������Ϣ���Ƶ�RTT
	_currMesh = _scene.GetMesh(0);
	_currMesh->visual.Reset();
	InitVisual(_currMesh, L"shader/Depth.fx");
	_currMesh = _scene.GetMesh(1);
	_currMesh->visual.Reset();
	InitVisual(_currMesh, L"shader/Depth.fx");



	//��Ⱦ�����ĳ���
	//�ع����
	_camera.SetPos(0, 0, -2.0f);
	_camera.SetEulerAngle(0, 0, 0);
	//����mesh��shaderΪ������Ⱦshader
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


	//���Ƶ��󻺳�
	_dxEngine.SetDefaultRenderTargetView();
	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());

	_dxEngine.Present();
}