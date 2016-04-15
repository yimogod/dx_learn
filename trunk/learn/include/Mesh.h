#pragma once

#include <iostream>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Color.h"
#include "Vertex.h"

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
	unsigned short vertexNum = 0;
	Vector3D vertexList[64];
	/* vertex color, each vertex has its color */
	Color vertexColorList[64];
	/*���㷨��*/
	Vector3D vertexNormalList[54];

	/*S, T, S, T, S, T, ��ͼ�����б�*/
	unsigned short uvNum = 0;
	Vector2D uvList[64];

	/* �������� */
	unsigned short indexNum = 0;
	unsigned short indexList[512];
	/* uv�������� */
	unsigned short uvIndexList[512];
public:
	/* mesh�������е����� */
	Vector3D position;
	void setWorldPos(float x, float y, float z);

	/*��ʽ������׼������vertext buff, ����ʹ��vbo*/
	/*length is index num*/
	void getVertexList(Vertex list[]);
	/*length is vertex num*/
	void getVertexListV2(Vertex list[]);

	void calVertexNormal();
};
