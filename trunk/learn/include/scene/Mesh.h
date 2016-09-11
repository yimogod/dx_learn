#pragma once

#include <iostream>
#include <math/algebra/Vector3D.h>
#include <math/algebra/Vector2D.h>
#include <math/algebra/Matrix4x4.h>
#include <graphics/Color.h>
#include <graphics/Vertex.h>

/* �ܶ����������ɵ������� */
class Mesh{
public:
    static const int STATE_ACTIVE = 1;
    static const int STATE_VISIBLE = 2;
    static const int STATE_CULLED = 4;
    
	Mesh();
	~Mesh();
    
    std::string name;
	/* mesh��״̬ */
	int state;

	/*�������*/
	int vertexNum = 0;
	Vector3D vertexList[256];
	/* vertex color, each vertex has its color */
	Color vertexColorList[256];
	/*S, T, S, T, S, T, ��ͼ�����б�*/
	int uvNum = 0;// == vertexnum
	Vector2D uvList[256];
	/*����<��>����, �������ڶ�����*/
	Vector3D normalList[256];
	/*������������������*/
	Vector3D tangentList[256];

	//��������
	int indexNum = 0;
	int indexList[512];
public:
	/*��ʽ������׼������vertext buff, ����ʹ��vbo*/
	/*length is vertex num*/
	void GetVertexList(Vertex list[]);
	void GetIndexList(int list[]);

	/*���㷨������*/
	void calVertexNormal();

public:
	/* mesh�������е����� */
	Vector3D position;
	void SetWorldPos(float x, float y, float z);
	void Move(float x, float y, float z);
	/* mesh�������е���ת, ��תֵ�ǻ��� */
	Vector3D eulerAngle;
	void rotateX(float x);
	void rotateY(float y);

	Matrix4x4 localToWorldMatrix();
};
