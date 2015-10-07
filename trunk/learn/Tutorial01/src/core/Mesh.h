#pragma once

#include <iostream>
#include "Vertex.h"
#include "Triangle.h"

/* 很多个三角形组成的三角网 */
class Mesh{
public:
    static const int STATE_ACTIVE = 1;
    static const int STATE_VISIBLE = 2;
    static const int STATE_CULLED = 4;
    
	Mesh();
    
    std::string name;
	/* mesh的状态 */
    int state;

	/* mesh在世界中的坐标 */
    Vertex position;
    
	/* 顶点个数 */
    int vertexNum;
	/* 原始顶点列表 */
    Vertex vertexLocalList[64];
    /* 管线中经过转化的顶点数据 */
	Vertex vertexTransList[64];
    
	/* 顶点索引个数 */
	int indexNum;
	int indexList[512];

	/* mesh包含的三角形个数 */
    int triangleNum;
    Triangle triangleList[128];
    
	/* 理论上uvNum == vertexNum */
	int uvNum;
	/* S, T, S, T, S, T */
	float uvList[512];

	/* 法线个数, TODO, 确认点发现还是面法线 */
	int normalNum;
	float normalList[1024];

public:
	void setWorldPos(float x, float y, float z);

	void setTriangleByVertex();
    void copyLocalVertexToTrans();
};
