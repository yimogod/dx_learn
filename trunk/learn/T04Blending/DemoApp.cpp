#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include <util/ObjParser.h>
#include <graphics/GeoCreater.h>

#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::loadContent(){
	initDevice();

	_scene.renderType = Scene::RENDER_TYPE_FRAME;

	Mesh *m = new Mesh();
	m->setWorldPos(0, 0, 2.0f);
	GeoCreater::createSprite(m);
	_scene.meshList[0] = m;

	m = new Mesh();
	m->setWorldPos(1.0f, 0, 10.0f);
	GeoCreater::createSprite(m);
	_scene.meshList[1] = m;

	_scene.meshNum = 2;

	_scene.camera = new Camera();
	_scene.camera->setPos(0, 0, -10.0f);
	_scene.camera->setFrustum(1.0f, 45.0f, 1.0f, 100.0f);
	_scene.camera->setAspect(_width, _height);

	/*准备shader数据*/
	CreateShaderInfo vs;
	vs.fileName = L"shader/blending.fx";
	vs.entryPoint = "VS";
	vs.shaderModel = "vs_4_0";
	CreateShaderInfo ps;
	ps.fileName = L"shader/blending.fx";
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
	createConstBuffer(&_constBuff, sizeof(ConstantBuffer));

	createAlphaBlendState();
	createRasterizerState(D3D11_FILL_SOLID, _solidRS);

	createTexture(getFullPathW("assets/t_01_a.dds").c_str());
	createTexture(getFullPathW("assets/t_02.dds").c_str());

	Mesh* mesh = _scene.getMesh(0);
	_vertices_1 = new Vertex[mesh->indexNum];
	mesh->getVertexList(_vertices_1);
	createVertexBuffer(_vertices_1, mesh->indexNum, 40 * 4);

	mesh = _scene.getMesh(1);
	_vertices_2 = new Vertex[mesh->indexNum];
	mesh->getVertexList(_vertices_2);
	createVertexBuffer(_vertices_2, mesh->indexNum * 40 * 4, &_vertBuff_2);

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

/*这里面有严重的内存泄漏*/
void DemoApp::render(){
	if(_context == NULL)return;
	_context->ClearRenderTargetView(_renderTargetView, Colors::MidnightBlue);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);
	_context->PSSetSamplers(0, 1, &_sampleState);

	Mesh* mesh;

	/*处理第二个mesh, 先画远处的, 不透明的, 这个不透明的就是透明的dest*/
	disableAlphaBlend();
	mesh = _scene.getMesh(1);
	_context->PSSetShaderResources(0, 1, &_resView[1]);
	bindVertexBuff(_vertBuff_2);
	_context->Draw(mesh->indexNum, 0);


	/*处理第一个mesh, 再修正为透明模式, 绘制透明的, 透明的就是src*/
	enableAlphaBlend();
	mesh = _scene.getMesh(0);
	_context->PSSetShaderResources(0, 1, &_resView[0]);
	bindVertexBuff();
	_context->Draw(mesh->indexNum, 0);

	_chain->Present(0, 0);
}