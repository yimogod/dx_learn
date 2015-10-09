#pragma once

#include <iostream>
#include "Vector3D.h"
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
	unsigned short vertexNum;
	/*������ɫ*/
	Vector3D vertexList[64];
    
	/* ������������ */
	unsigned short indexNum;
	unsigned short indexList[512];
	/*�������е���ɫ, uv, normal����indexһһƥ��. �ж��ٸ�index, ���ж��ٸ�color, uv, normal*/
	/*������ɫ*/
	Color colorList[64];

	/*S, T, S, T, S, T, ��ͼ�����б�*/
	float uvList[128];

public:
	/* mesh�������е����� */
	Vector3D position;
	void setWorldPos(float x, float y, float z);

	/*��ʽ������׼������vertext buff, ����ʹ��vbo*/
	void getVertexList(Vertex list[]);
};
