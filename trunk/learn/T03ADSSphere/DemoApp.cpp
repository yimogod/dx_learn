#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include <util/ObjParser.h>

#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::loadContent(){
	createDXInput();

	ObjParser reader;
	reader.read(getFullPath("assets/cube.obj").c_str(), &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;

	_scene.camera = new Camera();
	_scene.camera->setPos(0, 0, -2.0f);
	_scene.camera->setFrustum(1.0f, 45.0f, 1.0f, 100.0f);
	_scene.camera->setAspect(_width, _height);


	_scene.lightList[0] = new Light();
	_scene.lightList[0]->type = Light::TYPE_DIRECTION;
	_scene.lightList[0]->ambientColor = Color{ 0.0f, 0.0f, 0.0f, 0.3f };
	_scene.lightList[0]->diffuseColor = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[0]->specularColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	_scene.lightList[0]->dir = Vector3D(1.0f, 0.0f, 1.0f);
	
	_scene.lightList[1] = new Light();
	_scene.lightList[1]->type = Light::TYPE_POINT;
	_scene.lightList[1]->ambientColor = Color{ 0.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[1]->diffuseColor = Color{ 1.0f, 0.0f, 1.0f, 1.0f };
	_scene.lightList[1]->specularColor = Color{ 0.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[1]->pos = Vector3D(2.0f, 0.0f, 0.0f);
	_scene.lightList[1]->range = 200.0f;
	_scene.lightList[1]->kc = 0.0f;
	_scene.lightList[1]->kl = 1.0f;
	_scene.lightList[1]->kq = 0.0f;

	_scene.lightNum = 2;

	/*准备顶点缓冲数据*/
	Mesh* mesh = _scene.getMesh(0);
	Vertex *vertices = 0;
	vertices = new Vertex[mesh->indexNum];
	mesh->getVertexList(vertices);

	/*准备shader数据*/
	CreateShaderInfo vs;
	vs.fileName = L"shader/Phong.fx";
	vs.entryPoint = "VS";
	vs.shaderModel = "vs_4_0";
	CreateShaderInfo ps;
	ps.fileName = L"shader/Phong.fx";
	ps.entryPoint = "PS";
	ps.shaderModel = "ps_4_0";

	/*创建 layout*/
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	int numElements = ARRAYSIZE(layout);

	createDevice();
	
	//createRasterizerState(D3D11_FILL_WIREFRAME, _wireframeRS);
	//createRasterizerState(D3D11_FILL_SOLID, _wireframeRS);

	createShader(vs, ps, layout, numElements);
	createVertexBuffer(vertices, mesh->indexNum, 56 * 4);

	createConstBuffer(&_constBuff, sizeof(ConstantBuffer));
	createConstBuffer(&_phongBuff, sizeof(PhongBuffer));
	createSamplerState();
	createDepthState();

	createTexture(getFullPathW("assets/t_02.dds").c_str());
	
	delete(vertices);
	return true;
}

void DemoApp::unloadContent(){
	BaseApp::unloadContent();
}

void DemoApp::update(){
	UpdatePosByRMouse(_scene.camera, 0.001f);
	UpdatePosByLMouse(_scene.currMesh(), 0.001f);

	/*根据相机重新计算各个矩阵*/
	ConstantBuffer cb;
	cb.model = _scene.currMesh()->localToWorldMatrix().transpose();
	cb.view = _scene.camera->getWorldToCameraMatrix().transpose();
	cb.perspective = _scene.camera->getCameraToProjMatrix().transpose();
	_context->UpdateSubresource(_constBuff, 0, nullptr, &cb, 0, 0);

	Light* light = _scene.lightList[0];
	Color ac = light->ambientColor;
	Color dc = light->diffuseColor;
	Color sc = light->specularColor;
	Vector3D d = light->dir;

	PhongBuffer pb;
	pb.eyeWorldPos = Float4{ 0.0f, 0.0f, -1.0f, 1.0f };

	DirectionLight dl = DirectionLight{
		Float4A{ ac.r, ac.g, ac.b, ac.a },
		Float4A{ dc.r, dc.g, dc.b, dc.a },
		Float4A{ sc.r, sc.g, sc.b, sc.a },
		Float4{ d.x, d.y, d.z, 1.0f } };
	pb.directionLight = dl;

	light = _scene.lightList[1];
	ac = light->ambientColor;
	dc = light->diffuseColor;
	sc = light->specularColor;
	d = light->pos;
	PointLight pl = PointLight{
		Float4A{ ac.r, ac.g, ac.b, ac.a },
		Float4A{ dc.r, dc.g, dc.b, dc.a },
		Float4A{ sc.r, sc.g, sc.b, sc.a },
		Float4{ d.x, d.y, d.z, 1.0f },
		light->range,
		Float3{ light->kc, light->kl, light->kq } };
	pb.pointLight = pl;

	_context->UpdateSubresource(_phongBuff, 0, nullptr, &pb, 0, 0);
}

void DemoApp::render(){
	if(_context == NULL)return;
	_context->ClearRenderTargetView(_backBuffView, Colors::MidnightBlue);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	bindVertexBuff();

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);
	_context->PSSetConstantBuffers(1, 1, &_phongBuff);
	_context->PSSetShaderResources(0, _resViewNum, _resView);
	_context->PSSetSamplers(0, 1, &_sampleState);

	Mesh *m = _scene.getMesh(0);
	_context->Draw(m->indexNum, 0);

	_chain->Present(0, 0);
}