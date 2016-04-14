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
	unsigned short vertexNum = 0;
	Vector3D vertexList[64];
	/* vertex color, each vertex has its color */
	Color vertexColorList[64];
	/*���㷨��*/
	Vector3D vertexNormalList[54];

	/* ������������ */
	unsigned short indexNum = 0;
	unsigned short indexList[512];
	/*�������е���ɫ, uv, normal����indexһһƥ��. �ж��ٸ�index, ���ж��ٸ�color, uv, normal*/
	/*������ɫ*/
	Color indexColorList[512];

	/*S, T, S, T, S, T, ��ͼ�����б�*/
	/*uv�б���Ը������Ӧ�� Ҳ���Ը�index��Ӧ�� ���ַ���*/
	float uvList[1024];

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
