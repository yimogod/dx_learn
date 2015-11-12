#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include <math.h>

#include <Mesh.h>
#include "ColorHillApp.h"

using namespace DirectX;

ColorHillApp::ColorHillApp(){}

ColorHillApp::~ColorHillApp(){}

bool ColorHillApp::loadContent(){
	_scene.meshNum = 1;
	_scene.meshList[0] = new Mesh();
	Mesh* mesh = _scene.getMesh(0);

	createGrid(64.0f, 64.0f, 8.0f, (*mesh));
	mesh->setWorldPos(-32.0f, 0.0f, -32.0f);

	_scene.camera = new Camera(0, -1.0f, -60.0f, 0, 0, 0);
	_scene.camera->setProperty(1.0f, 45.0f, 1.0f, 1000.0f, _width, _height);

	/*准备顶点缓冲数据*/
	Vertex *vertices = new Vertex[mesh->indexNum];
	mesh->getVertexListV2(vertices);

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
	createDXInput();
	createShader(vs, ps, layout, numElements);
	createVertexBuffer(vertices, mesh->vertexNum);
	createIndexBuffer(mesh->indexList, mesh->indexNum);
	createConstBuffer(&_constBuff, sizeof(ConstantBuffer));

	delete(vertices);

	return true;
}

void ColorHillApp::unloadContent(){
	BaseApp::unloadContent();
}

void ColorHillApp::update(){
	UpdatePosByKeyboard(_scene.camera, 0.01f);

	/*根据相机重新计算各个矩阵*/
	world_to_camera = _scene.camera->getWorldToCameraMatrix();

	//float aspect = _scene.camera->aspect;
	camera_to_perspective = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	ConstantBuffer cb;
	cb.view = world_to_camera.transpose();
	cb.perspective = camera_to_perspective.transpose();
	_context->UpdateSubresource(_constBuff, 0, nullptr, &cb, 0, 0);
}

void ColorHillApp::render(){
	if(_context == NULL)return;
	_context->ClearRenderTargetView(_backBuffTarget, Colors::MidnightBlue);

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);

	Mesh *m = _scene.getMesh(0);
	_context->DrawIndexed(m->indexNum, 0, 0);

	_chain->Present(0, 0);
}

void ColorHillApp::createGrid(float width, float depth, float unitSize, Mesh &mesh){
	int row = (int)(depth / unitSize) + 1;
	int col = (int)(width / unitSize) + 1;

	mesh.vertexNum = (short)(row * col);

	int index = 0;
	float pz, px, py;
	for(int r = 0; r < row; ++r){
		for(int c = 0; c < col; ++c){
			index = r * row + c;
			pz = r * unitSize;
			px = c * unitSize;
			py = getVertexHeight(px, pz);
			mesh.vertexList[index].z = pz;
			mesh.vertexList[index].x = px;
			mesh.vertexList[index].y = py;

			mesh.vertexColorList[index] = getColorFromHeight(py);
		}
	}

	/*each rectange have 6 vertex*/
	mesh.indexNum = (short)((row - 1) * (col - 1) * 6);

	int index1 = 0;
	int index2 = 0;
	for(int r = 0; r < row; ++r){
		for(int c = 0; c < col; ++c){
			index = (r * row + c) * 6;

			index1 = r * row + c;
			index2 = index1 + row;
			
			mesh.indexList[index + 0] = index1;
			mesh.indexList[index + 1] = index1 + 1;
			mesh.indexList[index + 2] = index2;
			mesh.indexList[index + 3] = index2;
			mesh.indexList[index + 4] = index1 + 1;
			mesh.indexList[index + 5] = index2 + 1;
		}
	}
}

float ColorHillApp::getVertexHeight(float x, float z){
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}

Color ColorHillApp::getColorFromHeight(float y){
	if(y < -10.0f)return Color(1.0f, 0.96f, 0.62f);
	if(y < 5.0f)return Color(0.48f, 0.77f, 0.46f);
	if(y < 12.0f)return Color(0.1f, 0.48f, 0.19f);
	if(y < 20.0f)return Color(0.45f, 0.39f, 0.34f);
	return Color();
}