#include <util/ObjParser.h>
#include <graphics/GeoCreater.h>
#include "DemoApp.h"

DemoApp::DemoApp(){
	_noise.frame = 0;

	_noise.scroll.x = 1.3f;
	_noise.scroll.y = 2.1f;
	_noise.scroll.z = 2.3f;

	_noise.scale.x = 1.0f;
	_noise.scale.y = 2.0f;
	_noise.scale.z = 3.0f;

	_distortion.distortion1.x = 0.1f;
	_distortion.distortion1.y = 0.2f;
	_distortion.distortion2.x = 0.1f;
	_distortion.distortion2.y = 0.3f;
	_distortion.distortion3.x = 0.1f;
	_distortion.distortion3.y = 0.1f;

	_distortion.distortionScale = 0.8f;
	_distortion.distortionBias = 0.5f;
}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	//ObjParser reader;
	//reader.Read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	//_scene.renderType = Scene::RENDER_TYPE_FRAME;
	//_scene.camera = &_camera;
	//_currMesh = _scene.GetMesh(0);

	_currMesh = new Mesh();
	_currMesh->SetWorldPos(0, 0, 0.0f);
	GeoCreater::CreateSprite(*_currMesh);
	_scene.AddMesh(_currMesh);

	AddTexture(_currMesh, "assets/fire/fire01.dds");
	AddTexture(_currMesh, "assets/fire/noise01.dds");
	AddTexture(_currMesh, "assets/fire/alpha01.dds");
	InitVisual(_currMesh, L"shader/fire.fx");
	_dxEngine.EnableAlphaBlend();
	return true;
}

void DemoApp::PreAddOtherConstBuffer(DXVisual &visual){
	visual.PreSetVSConstBufferSize(sizeof(NoiseBuffer));
	visual.PreSetPSConstBufferSize(sizeof(DistortionBuffer));
}

void DemoApp::UnloadContent(){}

void DemoApp::Update(){
	Window::Update();

	_noise.frame += 0.001f;
	if(_noise.frame > 1000.0f)_noise.frame = 0.0f;

	_dxEngine.UpdateVSSubResource(GetVisual(), 1, &_noise);
	_dxEngine.UpdatePSSubResource(GetVisual(), 0, &_distortion);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}
