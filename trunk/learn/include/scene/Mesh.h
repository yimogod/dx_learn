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
	Vector3D vertexList[256];
	/* vertex color, each vertex has its color */
	Color vertexColorList[256];
	/*S, T, S, T, S, T, 贴图数据列表*/
	int uvNum = 0;// == vertexnum
	Vector2D uvList[256];
	/*顶点<点>法线, 数量等于定点数*/
	Vector3D normalList[256];
	/*切线数等于索引数量*/
	Vector3D tangentList[256];

	//顶点索引
	int indexNum = 0;
	int indexList[512];
public:
	/*格式化数据准备传入vertext buff, 我们使用vbo*/
	/*length is vertex num*/
	void GetVertexList(Vertex list[]);
	void GetIndexList(int list[]);

	/*计算法线数据*/
	void calVertexNormal();

public:
	/* mesh在世界中的坐标 */
	Vector3D position;
	void SetWorldPos(float x, float y, float z);
	void Move(float x, float y, float z);
	/* mesh在世界中的旋转, 旋转值是弧度 */
	Vector3D eulerAngle;
	void rotateX(float x);
	void rotateY(float y);

	Matrix4x4 localToWorldMatrix();
};
