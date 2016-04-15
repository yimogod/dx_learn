#pragma once

#include <iostream>
#include "Vector3D.h"
#include "Vector2D.h"
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
	unsigned short vertexNum = 0;
	Vector3D vertexList[64];
	/* vertex color, each vertex has its color */
	Color vertexColorList[64];
	/*顶点法线*/
	Vector3D vertexNormalList[54];

	/*S, T, S, T, S, T, 贴图数据列表*/
	unsigned short uvNum = 0;
	Vector2D uvList[64];

	/* 顶点索引 */
	unsigned short indexNum = 0;
	unsigned short indexList[512];
	/* uv坐标索引 */
	unsigned short uvIndexList[512];
public:
	/* mesh在世界中的坐标 */
	Vector3D position;
	void setWorldPos(float x, float y, float z);

	/*格式化数据准备传入vertext buff, 我们使用vbo*/
	/*length is index num*/
	void getVertexList(Vertex list[]);
	/*length is vertex num*/
	void getVertexListV2(Vertex list[]);

	void calVertexNormal();
};
