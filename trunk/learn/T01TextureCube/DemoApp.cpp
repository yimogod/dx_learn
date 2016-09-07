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
	Vertex *vertices = new Vertex[_currMesh->indexNum];
	_currMesh->getVertexList(vertices);

	/*准备shader数据*/
	Shader vs(L"shader/Triangle.fx", "VS", "vs_4_0");
	Shader ps(L"shader/Triangle.fx", "PS", "ps_4_0");
	
	/*创建 layout*/
	InputLayout layout;
	layout.AddElement("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
	layout.AddElement("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
	layout.AddElement("TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);

	
	_dxEngine.CreateShader(vs, ps, layout);
	_dxEngine.CreateVertexBuffer(vertices, _currMesh->indexNum, 40 * 4);
	_dxEngine.CreateConstBuffer(sizeof(ConstantBuffer));
	_dxEngine.CreateTexture(GetFullPathW("assets/t_01.dds").c_str());

	delete(vertices);
	return true;
}

void DemoApp::UnloadContent(){
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();

	_dxEngine.BindVertexBuff();
	_dxEngine.VSSetShader();
	_dxEngine.VSSetConstantBuffers(0, 1);
	_dxEngine.PSSetShader();
	_dxEngine.PSSetShaderResources(0);
	_dxEngine.PSSetSamplers(0, 1);

	_dxEngine.GetContext()->Draw(_currMesh->indexNum, 0);
	_dxEngine.Present();
}