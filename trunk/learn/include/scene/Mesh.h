#pragma once

#include <iostream>
#include <math/algebra/Vector3D.h>
#include <math/algebra/Vector2D.h>
#include <math/algebra/Matrix4x4.h>
#include <graphics/Color.h>
#include <graphics/Vertex.h>

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
	int vertexNum = 0;
	Vector3D vertexList[128];
	/* vertex color, each vertex has its color */
	Color vertexColorList[128];

	/*S, T, S, T, S, T, 贴图数据列表*/
	int uvNum = 0;
	Vector2D uvList[256];

	/* 顶点索引和uv坐标索引 */
	int indexNum = 0;
	int indexList[512];
	int uvIndexList[512];
	/*顶点面法线, 法线数等于索引数量, 或者顶点法线, 数量等于定点数*/
	Vector3D normalList[512];
	/*顶点法线, 切线数等于索引数量*/
	Vector3D tangentList[512];
public:
	/*格式化数据准备传入vertext buff, 我们使用vbo*/
	/*length is vertex num*/
	void GetVertexList(Vertex list[]);
	void GetIndexList(int list[]);

	/*  分别给顶点传入位置, 法线和uv数据
	 ** 其实就是拆分了getVertexList()
	 */
	void getVertexPosList(Vertex list[]);
	void getVertexUVList(Vertex list[]);
	void getVertexNormalList(Vertex list[]);

	/*计算法线数据*/
	void calVertexNormal();

public:
	/* mesh在世界中的坐标 */
	Vector3D position;
	void setWorldPos(float x, float y, float z);
	/* mesh在世界中的旋转, 旋转值是弧度 */
	Vector3D eulerAngle;
	void rotateX(float x);
	void rotateY(float y);

	Matrix4x4 localToWorldMatrix();
};
