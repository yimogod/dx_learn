#include <dinput.h>
#include <DirectXMath.h>
#include <util/ObjParser.h>
#include <DirectXColors.h>
#include <graphics/Mesh.h>
#include <graphics/Shader.h>
#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	ObjParser reader;
	reader.read(GetFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;

	_scene.camera = new Camera();
	_scene.camera->setPos(0, 0, -2.0f);
	_scene.camera->setFrustum(1.0f, 45.0f, 1.0f, 100.0f);
	_scene.camera->setAspect(_width, _height);
	
	/*准备顶点缓冲数据*/
	Mesh* mesh = _scene.getMesh(0);
	Vertex *vertices = 0;
	vertices = new Vertex[mesh->indexNum];
	mesh->getVertexList(vertices);

	/*准备shader数据*/
	Shader vs(L"shader/Triangle.fx", "VS", "vs_4_0");
	Shader ps(L"shader/Triangle.fx", "PS", "ps_4_0");

	/*创建 layout*/
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	int numElements = ARRAYSIZE(layout);
	_dxEngine.CreateShader(vs, ps, layout, numElements);
	_dxEngine.CreateVertexBuffer(vertices, mesh->indexNum, 40 * 4);
	_dxEngine.CreateConstBuffer(sizeof(ConstantBuffer));
	_dxEngine.CreateTexture(GetFullPathW("assets/t_01.dds").c_str());

	delete(vertices);
	return true;
}

void DemoApp::UnloadContent(){
}

void DemoApp::Update(){
	//UpdatePosByRMouse(_scene.camera, 0.001f);
	//UpdatePosByLMouse(_scene.currMesh(), 0.001f);

	ConstantBuffer cb;
	cb.model = _scene.currMesh()->localToWorldMatrix().transpose();
	cb.view = _scene.camera->getWorldToCameraMatrix().transpose();
	cb.perspective = _scene.camera->getCameraToProjMatrix().transpose();
	_dxEngine.GetContext()->UpdateSubresource(_dxEngine.GetConstBuff(), 0, nullptr, &cb, 0, 0);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearRenderTargetView(Colors::MidnightBlue);
	_dxEngine.ClearDepthStencilView(D3D11_CLEAR_DEPTH, 1.0f, 0);
	_dxEngine.BindVertexBuff();

	_dxEngine.VSSetShader();
	_dxEngine.VSSetConstantBuffers(0, 1);
	_dxEngine.PSSetShader();
	_dxEngine.PSSetShaderResources(0);
	_dxEngine.PSSetSamplers(0, 1);

	Mesh *m = _scene.getMesh(0);
	_dxEngine.GetContext()->Draw(m->indexNum, 0);
	_dxEngine.GetChain()->Present(0, 0);
}