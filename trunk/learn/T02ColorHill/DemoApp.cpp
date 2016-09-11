#include <math.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_camera.setEulerAngle(0, -1.2f, 0);
	_camera.setPos(0, 6.0f, -2.0f);

	_currMesh = new Mesh();
	CreateGrid(8, 8, (*_currMesh));
	//中心为原点
	_currMesh->SetWorldPos(-4.0f, 0.0f, -4.0f);

	/*准备顶点缓冲数据*/
	InitVisual(_visual, _currMesh, L"shader/ColorVertex.fx");
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

//cols, rows指的是网格线的个数, 注释以4x4网格为例子, 但只有9个正方形
void DemoApp::CreateGrid(int cols, int rows, Mesh &mesh){
	mesh.vertexNum = cols * rows;

	//会有16个顶点
	int index = 0;
	for(int row = 0; row < rows; ++row){
		for(int col = 0; col < cols; ++col){
			index = row * cols + col;
			int py = GetVertexHeight(col, row);
			mesh.vertexList[index].z = row;
			mesh.vertexList[index].x = col;
			mesh.vertexList[index].y = py;

			mesh.vertexColorList[index] = GetColorFromHeight(py);
		}
	}

	/*each rectange have 6 vertex*/
	mesh.indexNum = (cols - 1) * (rows - 1) * 6;

	//正方形底下那条边的左侧点的索引
	int lowIndex = 0;
	//正方形上面那条边的左侧点的索引
	int highIndex = 0;
	int top = rows - 1;
	int right = cols - 1;
	index = 0;
	for(int row = 0; row < top; ++row){
		for(int col = 0; col < right; ++col){
			lowIndex = row * cols + col;
			highIndex = lowIndex + cols;
			mesh.indexList[index + 0] = lowIndex;
			mesh.indexList[index + 1] = highIndex;
			mesh.indexList[index + 2] = lowIndex + 1;
			mesh.indexList[index + 3] = lowIndex + 1;
			mesh.indexList[index + 4] = highIndex;
			mesh.indexList[index + 5] = highIndex + 1;
			index += 6;
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