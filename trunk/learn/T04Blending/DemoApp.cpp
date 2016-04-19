#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include <ObjParser.h>

#include <Mesh.h>
#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::loadContent(){
	_scene.renderType = Scene::RENDER_TYPE_FRAME;
	char* sInputFile = "assets/simple_scene.obj";
	ObjParser reader;
	reader.read(sInputFile, &_scene);
	sInputFile = "assets/sphere.obj";
	reader.read(sInputFile, &_scene);

	_scene.getMesh(0)->setWorldPos(0, 10.0f, 10.0f);
	_scene.getMesh(0)->setWorldPos(1.0f, 2.0f, 2.0f);

	_scene.camera = new Camera();
	_scene.camera->setPos(0, 0, -2.0f);
	_scene.camera->setFrustum(1.0f, 45.0f, 1.0f, 100.0f);
	_scene.camera->setAspect(_width, _height);

	/*准备shader数据*/
	CreateShaderInfo vs;
	vs.fileName = L"shader/Triangle.fx";
	vs.entryPoint = "VS";
	vs.shaderModel = "vs_4_0";
	CreateShaderInfo ps;
	ps.fileName = L"shader/Triangle.fx";
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
	//createRasterizerState(D3D11_FILL_SOLID, _wireframeRS);

	createShader(vs, ps, layout, numElements);
	
	createConstBuffer(&_constBuff, sizeof(ConstantBuffer));

	createBlendState();
	//const wchar_t* path =
	//	L"E:/learn/dx_learn/trunk/learn/T04Blending/assets/t_2.dds";
	//createTexture(path);
	createSamplerState();
	return true;
}

void DemoApp::unloadContent(){
	BaseApp::unloadContent();
}

void DemoApp::update(){
	UpdatePosByKeyboard(_scene.camera, 0.001f);

	ConstantBuffer cb;
	cb.view = _scene.camera->getWorldToCameraMatrix().transpose();
	cb.perspective = _scene.camera->getCameraToProjMatrix().transpose();
	_context->UpdateSubresource(_constBuff, 0, nullptr, &cb, 0, 0);
}

/*这里面有严重的内存泄漏*/
void DemoApp::render(){
	if(_context == NULL)return;
	_context->ClearRenderTargetView(_backBuffView, Colors::MidnightBlue);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);
	_context->PSSetSamplers(0, 1, &_sampleState);

	/*处理第一个mesh*/
	const wchar_t* path =
		L"E:/learn/dx_learn/trunk/learn/T04Blending/assets/t_2.dds";
	createTexture(path);

	/*给shader传送贴图资源, 可以传多张贴图*/
	_context->PSSetShaderResources(0, 1, &_resView);
	Mesh* mesh = _scene.getMesh(0);
	Vertex *vertices = new Vertex[mesh->indexNum];
	mesh->getVertexList(vertices);

	createVertexBuffer(vertices, mesh->indexNum);
	delete(vertices);
	_context->Draw(mesh->indexNum, 0);

	const wchar_t* path1 =
		L"E:/learn/dx_learn/trunk/learn/T04Blending/assets/t_1.dds";
	createTexture(path1);
	_context->PSSetShaderResources(0, 1, &_resView);

	/*处理第二个mesh*/
	mesh = _scene.getMesh(1);
	vertices = new Vertex[mesh->indexNum];
	mesh->getVertexList(vertices);

	createVertexBuffer(vertices, mesh->indexNum);
	delete(vertices);
	_context->Draw(mesh->indexNum, 0);

	_chain->Present(0, 0);
}