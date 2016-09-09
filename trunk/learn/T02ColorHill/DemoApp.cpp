#include <math.h>
#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_scene.camera = &_camera;

	_scene.meshNum = 1;
	_scene.meshList[0] = new Mesh();
	_currMesh = _scene.getMesh(0);

	CreateGrid(128.0f, 128.0f, 8.0f, (*_currMesh));
	_currMesh->setWorldPos(-64.0f, 0.0f, -64.0f);

	/*准备顶点缓冲数据*/
	Vertex *vertices = new Vertex[_currMesh->indexNum];
	_currMesh->getVertexPosList(vertices);

	InitVisual(_visual, L"shader/ColorVertex.fx", vertices);

	//createRasterizerState(D3D11_FILL_WIREFRAME, _wireframeRS);

	delete(vertices);
	return true;
}

void DemoApp::UnloadContent(){
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(_visual);
	_dxEngine.Present();
}

void DemoApp::CreateGrid(float width, float depth, float unitSize, Mesh &mesh){
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
			py = GetVertexHeight(px, pz);
			mesh.vertexList[index].z = pz;
			mesh.vertexList[index].x = px;
			mesh.vertexList[index].y = py;

			mesh.vertexColorList[index] = GetColorFromHeight(py);
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

float DemoApp::GetVertexHeight(float x, float z){
	return 0.1f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}

Color DemoApp::GetColorFromHeight(float y){
	if(y < -20.0f)return Color(1.0f, 0.96f, 0.62f);
	if(y < 5.0f)return Color(0.48f, 0.77f, 0.46f);
	if(y < 12.0f)return Color(0.1f, 0.48f, 0.19f);
	if(y < 20.0f)return Color(0.45f, 0.39f, 0.34f);
	return Color();
}