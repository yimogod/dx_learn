#pragma once

#include <iostream>
#include "Vertex.h"
#include "Triangle.h"

/* �ܶ����������ɵ������� */
class Mesh{
public:
    static const int STATE_ACTIVE = 1;
    static const int STATE_VISIBLE = 2;
    static const int STATE_CULLED = 4;
    
	Mesh();
    
    std::string name;
	/* mesh��״̬ */
    int state;

	/* mesh�������е����� */
    Vertex position;
    
	/* ������� */
    int vertexNum;
	/* ԭʼ�����б� */
    Vertex vertexLocalList[64];
    /* �����о���ת���Ķ������� */
	Vertex vertexTransList[64];
    
	/* ������������ */
	int indexNum;
	int indexList[512];

	/* mesh�����������θ��� */
    int triangleNum;
    Triangle triangleList[128];
    
	/* ������uvNum == vertexNum */
	int uvNum;
	/* S, T, S, T, S, T */
	float uvList[512];

	/* ���߸���, TODO, ȷ�ϵ㷢�ֻ����淨�� */
	int normalNum;
	float normalList[1024];

public:
	void setWorldPos(float x, float y, float z);

	void setTriangleByVertex();
    void copyLocalVertexToTrans();
};
