#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include <math.h>

#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::loadContent(){
	_scene.meshNum = 1;
	_scene.meshList[0] = new Mesh();
	Mesh* mesh = _scene.getMesh(0);

	//创建鼠标键盘放在前面, 是防止debug时老报错
	createDXInput();

	createGrid(128.0f, 128.0f, 8.0f, (*mesh));
	mesh->setWorldPos(-64.0f, 0.0f, -64.0f);

	_scene.camera = new Camera();
	_scene.camera->setPos(0, 50.0f, -50.0f);
	_scene.camera->setFrustum(1.0f, 45.0f, 1.0f, 100.0f);
	_scene.camera->setEulerAngle(0.0f, -0.5f, 0.0f);
	_scene.camera->setAspect(_width, _height);

	/*准备顶点缓冲数据*/
	Vertex *vertices = new Vertex[mesh->indexNum];
	mesh->getVertexPosList(vertices);

	/*准备shader数据*/
	CreateShaderInfo vs;
	vs.fileName = L"shader/ColorVertex.fx";
	vs.entryPoint = "VS";
	vs.shaderModel = "vs_4_0";
	CreateShaderInfo ps;
	ps.fileName = L"shader/ColorVertex.fx";
	ps.entryPoint = "PS";
	ps.shaderModel = "ps_4_0";

	/*创建 layout*/
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int numElements = ARRAYSIZE(layout);

	createDevice();
	
	createShader(vs, ps, layout, numElements);
	createVertexBuffer(vertices, mesh->indexNum);
	createConstBuffer(&_constBuff, sizeof(ConstantBuffer));
	//createRasterizerState(D3D11_FILL_WIREFRAME, _wireframeRS);

	delete(vertices);

	return true;
}

void DemoApp::unloadContent(){
	BaseApp::unloadContent();
}

void DemoApp::update(){
	UpdatePosByRMouse(_scene.camera, 0.001f);

	ConstantBuffer cb;
	cb.view = _scene.camera->getWorldToCameraMatrix().transpose();
	cb.perspective = _scene.camera->getWorldToProjMatrix().transpose();
	_context->UpdateSubresource(_constBuff, 0, nullptr, &cb, 0, 0);
}

void DemoApp::render(){
	if(_context == NULL)return;
	_context->ClearRenderTargetView(_backBuffView, Colors::MidnightBlue);

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);

	Mesh *m = _scene.getMesh(0);
	_context->Draw(m->indexNum, 0);

	_chain->Present(0, 0);
}

void DemoApp::createGrid(float width, float depth, float unitSize, Mesh &mesh){
	int rowVertex = (int)(depth / unitSize) + 1;
	int colVertex = (int)(width / unitSize) + 1;

	mesh.vertexNum = (short)(rowVertex * colVertex);

	int index = 0;
	float pz, px, py;
	for(int r = 0; r < rowVertex; ++r){
		pz = r * unitSize;
		for(int c = 0; c < colVertex; ++c){
			index = r * colVertex + c;
			px = c * unitSize;
			py = 0.0f;
			py = getVertexHeight(px, pz);
			mesh.vertexList[index].z = pz;
			mesh.vertexList[index].x = px;
			mesh.vertexList[index].y = py;

			mesh.vertexColorList[index] = getColorFromHeight(py);
		}
	}

	/*each rectange have 6 vertex*/
	mesh.indexNum = (short)((rowVertex - 1) * (colVertex - 1) * 6);

	//正方形底下那条边的左侧点的索引
	int lowIndex = 0;
	//正方形上面那条边的左侧点的索引
	int highIndex = 0;
	int top = rowVertex - 1;
	int right = colVertex - 1;
	for(int r = 0; r < top; ++r){
		for(int c = 0; c < right; ++c){
			index = (r * right + c) * 6;

			lowIndex = r * colVertex + c;
			highIndex = lowIndex + colVertex;
			
			mesh.indexList[index + 0] = lowIndex;
			mesh.indexList[index + 1] = highIndex;
			mesh.indexList[index + 2] = lowIndex + 1;
			mesh.indexList[index + 3] = lowIndex + 1;
			mesh.indexList[index + 4] = highIndex;
			mesh.indexList[index + 5] = highIndex + 1;
		}
	}
}

float DemoApp::getVertexHeight(float x, float z){
	return 0.1f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}

Color DemoApp::getColorFromHeight(float y){
	if(y < -20.0f)return Color(1.0f, 0.96f, 0.62f);
	if(y < 5.0f)return Color(0.48f, 0.77f, 0.46f);
	if(y < 12.0f)return Color(0.1f, 0.48f, 0.19f);
	if(y < 20.0f)return Color(0.45f, 0.39f, 0.34f);
	return Color();
}