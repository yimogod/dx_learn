#pragma once

#include <iostream>
#include <BaseDataStruct.h>
#include <graphics/DXVisual.h>
#include <math/algebra/Vector3D.h>
#include <math/algebra/Vector2D.h>
#include <math/algebra/Matrix4x4.h>

/* 很多个三角形组成的三角网 */
class Mesh{
public:
    static const int STATE_ACTIVE = 1;
    static const int STATE_VISIBLE = 2;
    static const int STATE_CULLED = 4;
    
	Mesh();
	~Mesh();
    
	DXVisual visual;

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

	//顶点索引, 如果=0, 那我们就不用drawindex了.
	int indexNum = 0;
	int indexList[512];
public:
	/*格式化数据准备传入vertext buff, 我们使用vbo*/
	/*length is vertex num*/
	void GetVertexList(Vertex list[]);
	void GetIndexList(int list[]);

	/*计算法线数据*/
	void CalVertexNormal();

public:
	/* mesh在世界中的坐标 */
	Vector3D position;
	void SetWorldPos(float x, float y, float z);
	void SetScale(float scale);
	void Move(float x, float y, float z);
	/* mesh在世界中的旋转, 旋转值是弧度 */
	Vector3D eulerAngle;
	void RotateX(float x);
	void RotateY(float y);

	//顶点颜色
	void SetColor(Color& color);

	Matrix4x4 localToWorldMatrix();
	inline bool UseIndex() const;
};

inline bool Mesh::UseIndex() const{
	return indexNum > 0;
}