#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include <graphics/GeoCreater.h>

#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

static bool use_index = false;

bool DemoApp::loadContent(){
	initDevice();

	Mesh *m = new Mesh();
	m->setWorldPos(0, 0, 0.0f);
	GeoCreater::CreateSprite(m);
	_scene.meshList[0] = m;
	_scene.meshNum = 1;

	_scene.camera = new Camera();
	_scene.camera->setPos(0, 0, -2.0f);
	_scene.camera->setFrustum(1.0f, 45.0f, 1.0f, 100.0f);
	_scene.camera->setAspect(_width, _height);
	
	/*准备顶点缓冲数据*/
	Mesh* mesh = _scene.getMesh(0);
	Vertex *vertices = 0;
	if(use_index){
		vertices = new Vertex[mesh->vertexNum];
		mesh->getVertexList_v2(vertices);
	}else{
		vertices = new Vertex[mesh->indexNum];
		mesh->getVertexList(vertices);
	}

	/*准备shader数据*/
	CreateShaderInfo vs;
	vs.fileName = L"shader/alpha_map.fx";
	vs.entryPoint = "VS";
	vs.shaderModel = "vs_4_0";
	CreateShaderInfo ps;
	ps.fileName = L"shader/alpha_map.fx";
	ps.entryPoint = "PS";
	ps.shaderModel = "ps_4_0";

	/*创建 layout*/
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int numElements = ARRAYSIZE(layout);

	createShader(vs, ps, layout, numElements);
	if(use_index){
		createVertexBuffer(vertices, mesh->vertexNum, 40 * 4);
		createIndexBuffer(mesh->indexList, mesh->indexNum);
	}else{
		createVertexBuffer(vertices, mesh->indexNum, 40 * 4);
	}
	createConstBuffer(&_constBuff, sizeof(ConstantBuffer));

	createTexture(getFullPathW("assets/t_01.dds").c_str());
	createTexture(getFullPathW("assets/t_02.dds").c_str());
	createTexture(getFullPathW("assets/a_01.dds").c_str());

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
	_context->ClearRenderTargetView(_renderTargetView, Colors::MidnightBlue);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	bindVertexBuff();

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);
	_context->PSSetShaderResources(0, _resViewNum, _resView);
	_context->PSSetSamplers(0, 1, &_sampleState);

	Mesh *m = _scene.getMesh(0);
	if(use_index){
		_context->DrawIndexed(m->indexNum, 0, 0);
	}else{
		_context->Draw(m->indexNum, 0);
	}

	_chain->Present(0, 0);
}