#pragma once

#include <iostream>
#include <math/algebra/Vector3D.h>
#include <math/algebra/Vector2D.h>
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
	unsigned short vertexNum = 0;
	Vector3D vertexList[128];
	/* vertex color, each vertex has its color */
	Color vertexColorList[128];
	

	/*S, T, S, T, S, T, ��ͼ�����б�*/
	unsigned short uvNum = 0;
	Vector2D uvList[256];

	/* ����������uv�������� */
	unsigned short indexNum = 0;
	unsigned short indexList[512];
	unsigned short uvIndexList[512];
	/*���㷨��, ������������������*/
	Vector3D vertexNormalList[512];
public:
	/* mesh�������е����� */
	Vector3D position;
	void setWorldPos(float x, float y, float z);

	/*��ʽ������׼������vertext buff, ����ʹ��vbo*/
	/*length is index num*/
	void getVertexList(Vertex list[]);
	/*length is vertex num*/
	void getVertexList_v2(Vertex list[]);

	/*  �ֱ�����㴫��λ��, ���ߺ�uv����
	 ** ��ʵ���ǲ����getVertexList()
	 */
	void getVertexPosList(Vertex list[]);
	void getVertexUVList(Vertex list[]);
	void getVertexNormalList(Vertex list[]);

	void calVertexNormal();
};