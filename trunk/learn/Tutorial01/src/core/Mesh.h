#pragma once

#include <iostream>
#include "Vector3D.h"
#include "Color.h"
#include "Vertex.h"

/* 很多个三角形组成的三角网 */
class Mesh{
public:
    static const int STATE_ACTIVE = 1;
    static const int STATE_VISIBLE = 2;
    static const int STATE_CULLED = 4;
    
	Mesh();
	~Mesh();
    
    std::string name;
	/* mesh的状态 */
	int state;

	/*顶点个数*/
	int vertexNum;
	/*顶点颜色*/
	Vector3D vertexList[64];
    
	/*顶点颜色*/
	Color colorList[64];

	/*S, T, S, T, S, T, 贴图数据列表*/
	float uvList[128];

	/* 顶点索引个数 */
	int indexNum;
	int indexList[512];

public:
	/* mesh在世界中的坐标 */
	Vector3D position;
	void setWorldPos(float x, float y, float z);

	/*格式化数据准备传入vertext buff, 我们使用vbo*/
	void getVertexList(Vertex list[]);
};
