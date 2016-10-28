#include <graphics/GeoCreater.h>
#include "DemoApp.h"
//�и�������Ҫע��һ��. �������blend, ��ô���Ȼ��Ʋ�͸������
//����, ͸�������Կձ�������blend, Ȼ���ٻ��Ʋ�͸������Ļ�, �ʹﲻ��alphaЧ��
//����ζ��, ����������Ҫ��͸�������������,
//�Ȼ��Ʋ�͸��, �ٻ���͸��, Ȼ����Ż��Ƹ�����͸�����߲�͸��
DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_dxEngine.EnableAlphaBlend();
	
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(0, 0, 0.0f);
	GeoCreater::CreateSprite(*_currMesh);
	_currMesh->SetColor(Color(1.0f, 1.0f, 1.0f, 0.6f));
	_scene.AddMesh(_currMesh);
	InitVisual(_currMesh, L"shader/blending.fx", "assets/t_01.dds");

	//�����ԶЩ
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(1.0f, 0, 2.0f);
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);
	InitVisual(_currMesh, L"shader/blending.fx", "assets/t_02.dds");

	//�������Щ
	_currMesh = new Mesh();
	_currMesh->SetWorldPos(-0.6f, 0, 2.2f);
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);
	InitVisual(_currMesh, L"shader/blending.fx", "assets/t_01.dds");
	return true;
}

void DemoApp::UnloadContent(){}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	//ע��, ����˳��, ���Ȼ��Ʋ�͸��, �ٻ���͸��
	_dxEngine.ClearBuffers();
	_currMesh = _scene.meshList[2];
	_dxEngine.DrawVisual(GetVisual());
	_currMesh = _scene.meshList[1];
	_dxEngine.DrawVisual(GetVisual());

	_currMesh = _scene.meshList[0];
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}