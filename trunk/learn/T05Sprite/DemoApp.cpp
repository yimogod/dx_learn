#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>

#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::loadContent(){
	Mesh *m = new Mesh();
	m->setWorldPos(0, 0, 0.0f);
	m->vertexNum = 4;
	m->vertexList[0] = Vector3D(-1.0f, 1.0f, 0.0f);
	m->vertexList[1] = Vector3D(-1.0f, -1.0f, 0.0f);
	m->vertexList[2] = Vector3D(1.0f, -1.0f, 0.0f);
	m->vertexList[3] = Vector3D(1.0f, 1.0f, 0.0f);

	m->uvNum = 4;
	m->uvList[0] = Vector2D(0, 0);
	m->uvList[1] = Vector2D(0, 1.0f);
	m->uvList[2] = Vector2D(1.0f, 1.0f);
	m->uvList[3] = Vector2D(1.0f, 0);

	m->indexNum = 6;
	m->indexList[0] = 1;
	m->indexList[1] = 0;
	m->indexList[2] = 2;
	m->indexList[3] = 2;
	m->indexList[4] = 0;
	m->indexList[5] = 3;

	m->uvIndexList[0] = 1;
	m->uvIndexList[1] = 0;
	m->uvIndexList[2] = 2;
	m->uvIndexList[3] = 2;
	m->uvIndexList[4] = 0;
	m->uvIndexList[5] = 3;

	_scene.meshList[0] = m;
	_scene.meshNum = 1;

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
	CreateShaderInfo vs;
	vs.fileName = L"shader/sprite.fx";
	vs.entryPoint = "VS";
	vs.shaderModel = "vs_4_0";
	CreateShaderInfo ps;
	ps.fileName = L"shader/sprite.fx";
	ps.entryPoint = "PS";
	ps.shaderModel = "ps_4_0";

	/*创建 layout*/
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int numElements = ARRAYSIZE(layout);

	createDevice();
	createDXInput();
	//createRasterizerState(D3D11_FILL_WIREFRAME, _wireframeRS);
	//createRasterizerState(D3D11_FILL_SOLID, _solidRS);

	createShader(vs, ps, layout, numElements);
	createVertexBuffer(vertices, mesh->indexNum);
	createConstBuffer(&_constBuff, sizeof(ConstantBuffer));
	//createSamplerState();
	createTexture(getFullPathW("assets/t_01.dds").c_str());

	delete(vertices);
	return true;
}

void DemoApp::unloadContent(){
	BaseApp::unloadContent();
}

void DemoApp::update(){
	UpdatePosByRMouse(_scene.camera, 0.001f);
	UpdatePosByLMouse(_scene.currMesh(), 0.001f);

	ConstantBuffer cb;
	cb.model = _scene.currMesh()->localToWorldMatrix().transpose();
	cb.view = _scene.camera->getWorldToCameraMatrix().transpose();
	cb.perspective = _scene.camera->getCameraToProjMatrix().transpose();
	_context->UpdateSubresource(_constBuff, 0, nullptr, &cb, 0, 0);
}

void DemoApp::render(){
	if(_context == NULL)return;
	_context->ClearRenderTargetView(_backBuffView, Colors::MidnightBlue);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_vertexBuff, &stride, &offset);
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);
	_context->PSSetShaderResources(0, _resViewNum, _resView);
	//_context->PSSetSamplers(0, 1, &_sampleState);

	Mesh *m = _scene.getMesh(0);
	_context->Draw(m->indexNum, 0);

	_chain->Present(0, 0);
}