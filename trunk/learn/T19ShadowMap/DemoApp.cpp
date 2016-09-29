#include <util/ObjParser.h>
#include <graphics/GeoCreater.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_camera.SetPos(0, 1.0f, -4.0);
	_scene.camera = &_camera;

	//ƽ�й������ڵ�ͼ���Ͻ�, �����·��ۿ�
	_scene.lightList[0] = new Light();
	_scene.lightList[0]->type = Light::TYPE_DIRECTION;
	_scene.lightList[0]->ambientColor = Color{ 0.0f, 0.0f, 0.0f, 0.3f };
	_scene.lightList[0]->diffuseColor = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[0]->specularColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	_scene.lightList[0]->pos = Vector3D(-4.0f, 4.0f, 0.0f);
	_scene.lightList[0]->dir = Vector3D(1.0f, -1.0f, 0.0f);
	_scene.lightNum = 1;


	//1. ����, ������0, 0.5, 0
	ObjParser reader;
	reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_currMesh = _scene.GetMesh(0);
	_currMesh->SetWorldPos(0, 0, 0);
	PreSetVSConstBufferSize(_currMesh, sizeof(MVPConstBuffer));
	InitVisual(_currMesh, L"shader/Phong.fx", "assets/t_02.dds");

	//2. �ذ�, ������0, -0.5f, 0
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

	//1. ���������̬���ƹ���̬
	Vector3D oriPos = _camera.position;
	float head = _camera.heading;
	float pitch = _camera.pitch;
	float bank = _camera.bank;

	Light &light = *_scene.lightList[0];
	_camera.SetPos(light.pos);
	_camera.SetAspect(1280, 1280);
	_camera.SetEulerAngle(-1.6f, -0.5f, 0);

	//����light space��ת������
	_lightSpaceBuffer.view = _camera.GetWorldToCameraMatrix().transpose();
	_lightSpaceBuffer.perspective = _camera.GetCameraToProjMatrix().transpose();
	_lightSpaceBuffer.pos = Float4(light.pos.x, light.pos.y, light.pos.z, 1.0f);


	//2. ����mesh��shaderΪdepth, �������������Ϣ���Ƶ�RTT
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
	//����mvp
	UpdateConstBuff();
	_currMesh = _scene.GetMesh(0);
	_dxEngine.DrawVisual(GetVisual());
	_currMesh = _scene.GetMesh(1);
	_dxEngine.DrawVisual(GetVisual());

	//��Ⱦ�����ĳ���
	//�ع����
	_camera.SetPos(oriPos);
	_camera.SetEulerAngle(head, pitch, bank);
	_camera.SetAspect(_width, _height);

	//����mesh��shaderΪ������Ⱦshader
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

	//���Ƶ��󻺳�
	_dxEngine.SetDefaultRenderTargetView();
	_dxEngine.ClearBuffers();
	_currMesh = _scene.GetMesh(0);
	_dxEngine.DrawVisualByRTT(GetVisual());
	_currMesh = _scene.GetMesh(1);
	_dxEngine.DrawVisualByRTT(GetVisual());
	_dxEngine.Present();
}