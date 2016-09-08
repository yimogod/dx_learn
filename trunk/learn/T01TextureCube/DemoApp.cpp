#include <util/ObjParser.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	_scene.camera = &_camera;
	
	/*准备顶点缓冲数据*/
	_currMesh = _scene.getMesh(0);
	Vertex* vertices = new Vertex[_currMesh->indexNum];
	_currMesh->getVertexList(vertices);

	/*准备shader数据*/
	_visual.PreInitShader(L"shader/Triangle.fx", L"shader/Triangle.fx");
	
	/*创建 layout*/
	_visual.PreAddDefaultLayout();

	//创建buffer需要的变量
	_visual.PreSetConstBufferSize(sizeof(ConstantBuffer));
	
	//初始化visual
	_dxEngine.InitVisual(_visual, vertices, _currMesh->indexNum);
	
	_dxEngine.CreateTexture(GetFullPathW("assets/t_01.dds").c_str());

	delete(vertices);
	return true;
}

void DemoApp::UnloadContent(){
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(_visual);

	_dxEngine.PSSetShaderResources(0);
	_dxEngine.PSSetSamplers(0, 1);

	_dxEngine.GetContext()->Draw(_currMesh->indexNum, 0);
	_dxEngine.Present();
}