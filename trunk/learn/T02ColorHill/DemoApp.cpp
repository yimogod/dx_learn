#include <math.h>
#include "DemoApp.h"

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_camera.SetEulerAngle(0, -1.2f, 0);
	_camera.SetPos(0, 6.0f, -2.0f);

	_currMesh = new Mesh();
	CreateGrid(10, 10, (*_currMesh));
	//������ɽ��,����Ϊԭ��
	_currMesh->SetWorldPos(-4.0f, 0.0f, -4.0f);
	_scene.AddMesh(_currMesh);

	/*׼�����㻺������*/
	InitVisual(_currMesh, L"shader/ColorVertex.fx");
	return true;
}

void DemoApp::UnloadContent(){
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.DrawVisual(GetVisual());
	_dxEngine.Present();
}

//cols, rowsָ���������ߵĸ���, ע����4x4����Ϊ����, ��ֻ��9��������
void DemoApp::CreateGrid(int cols, int rows, Mesh &mesh){
	mesh.vertexNum = cols * rows;

	//����16������
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

	//�����ε��������ߵ����������
	int lowIndex = 0;
	//���������������ߵ����������
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
	return 0.4f * (z * sinf(0.4f * x) + x * cosf(0.4f * z));
}

Color DemoApp::GetColorFromHeight(float y){
	if(y < -5.0f)return Color(1.0f, 0.96f, 0.62f);
	if(y < 1.0f)return Color(0.48f, 0.77f, 0.46f);
	if(y < 3.0f)return Color(0.1f, 0.48f, 0.19f);
	if(y < 6.0f)return Color(0.45f, 0.39f, 0.34f);
	return Color();
}