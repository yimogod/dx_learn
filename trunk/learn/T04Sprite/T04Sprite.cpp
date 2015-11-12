#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>

#include <Mesh.h>
#include "T04Sprite.h"

using namespace DirectX;

T04Sprite::T04Sprite(){}

T04Sprite::~T04Sprite(){}

bool T04Sprite::loadContent(){
	const wchar_t* path =
		L"E:/learn/dx_learn/trunk/learn/T04Sprite/assets/seafloor.dds";

	Mesh *m = new Mesh();
	m->setWorldPos(0, 0, 0.0f);
	m->vertexNum = 4;
	m->vertexList[0] = Vector3D(-1.0f, 1.0f, 0.0f);
	m->vertexList[1] = Vector3D(-1.0f, -1.0f, 0.0f);
	m->vertexList[2] = Vector3D(1.0f, -1.0f, 0.0f);
	m->vertexList[3] = Vector3D(1.0f, 1.0f, 0.0f);
	
	m->uvList[0] = 0;
	m->uvList[1] = 1;
	m->uvList[2] = 0;
	m->uvList[3] = 0;
	m->uvList[4] = 1;
	m->uvList[5] = 0;
	m->uvList[6] = 1;
	m->uvList[7] = 1;

	/*dx默认三角形正方形是顺时针, 与gl相反*/
	m->indexNum = 6;
	m->indexList[0] = 1;
	m->indexList[1] = 0;
	m->indexList[2] = 2;
	m->indexList[3] = 2;
	m->indexList[4] = 0;
	m->indexList[5] = 3;


	_scene.meshList[0] = m;
	_scene.meshNum = 1;

	_scene.camera = new Camera(0, -1.0f, -2.0f, 0, 0, 0);
	_scene.camera->setProperty(1.0f, 45.0f, 1.0f, 100.0f, _width, _height);
	
	/*准备顶点缓冲数据*/
	Mesh* mesh = _scene.getMesh(0);
	Vertex *vertices = new Vertex[mesh->vertexNum];
	mesh->getVertexListV2(vertices);

	/*准备shader数据*/
	CreateShaderInfo vs;
	vs.fileName = L"shader/Sprite.fx";
	vs.entryPoint = "VS";
	vs.shaderModel = "vs_4_0";
	CreateShaderInfo ps;
	ps.fileName = L"shader/Sprite.fx";
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
	createShader(vs, ps, layout, numElements);
	createVertexBuffer(vertices, mesh->indexNum);
	createIndexBuffer(mesh->indexList, mesh->indexNum);
	createConstBuffer(&_constBuff, sizeof(ConstantBuffer));
	createTexture(path);
	
	delete(vertices);

	float aspect = _scene.camera->aspect;
	camera_to_perspective =Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, aspect, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 1.0f,
									0.0f, 0.0f, 0.0f, 0.0f);

	return true;
}

void T04Sprite::unloadContent(){
	BaseApp::unloadContent();
}

void T04Sprite::update(){
	UpdatePosByKeyboard(_scene.camera, 0.001f);

	/*根据相机重新计算各个矩阵*/
	world_to_camera = _scene.camera->getWorldToCameraMatrix();

	ConstantBuffer cb;
	cb.view = world_to_camera.transpose();
	cb.perspective = camera_to_perspective.transpose();
	_context->UpdateSubresource(_constBuff, 0, nullptr, &cb, 0, 0);
}

void T04Sprite::render(){
	if(_context == NULL)return;
	_context->ClearRenderTargetView(_backBuffTarget, Colors::MidnightBlue);
	

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);
	_context->PSSetShaderResources(0, 1, &_resView);
	_context->PSSetSamplers(0, 1, &_sampleState);

	Mesh *m = _scene.getMesh(0);
	_context->DrawIndexed(m->indexNum, 0, 0);

	_chain->Present(0, 0);
}