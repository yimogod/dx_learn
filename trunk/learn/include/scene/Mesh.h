#pragma once
#include <memory>
#include <iostream>
#include <BaseDataStruct.h>
#include <graphics/DXVisual.h>
#include <graphics/Material.h>
#include <math/algebra/Vector3D.h>
#include <math/algebra/Vector2D.h>
#include <math/algebra/Matrix4x4.h>
#include <scene/Transform.h>

class Transform;

/* 很多个三角形组成的三角网 */
class Mesh{
public:
    static const int STATE_ACTIVE = 1;
    static const int STATE_VISIBLE = 2;
    static const int STATE_CULLED = 4;
    
	Mesh();
	Mesh(const std::string path);
	~Mesh();
    
	DXVisual visual;
	//一个material会被不同的mesh引用
	std::shared_ptr<Material> material = nullptr;

    std::string name;
	/* mesh的状态 */
	int state;

	/*顶点个数*/
	int vertexNum = 0;
	//汇总的完整顶点数据
	Vertex* vertexList = nullptr;

	//位置坐标
	Vector3D posList[256];
	/*S, T, S, T, S, T, 贴图数据列表*/
	int uvNum = 0;// == vertexnum
	Vector2D uvList[256];
	/*顶点<点>法线, 数量等于顶点数*/
	Vector3D normalList[256];
	/*切线数等于索引数量*/
	Vector3D tangentList[256];
	

	//顶点索引, 如果=0, 那我们就不用drawindex了.
	int indexNum = 0;
	int indexList[512];
public:
	void InitStatic(Transform& trans);
	void Dispose();
	void UpdateCB(const void* data);//update const buffer

	/*计算法线数据*/
	void CalVertexNormal();
	//添加顶点位置坐标. 顶点个数也要+1
	void AddVertexPos(Vector3D& value);
	void AddUVPos(Vector2D& value);
private:
	/*格式化数据准备传入vertext buff, 我们使用vbo*/
	/*length is vertex num*/
	void GetVertexList(Transform& trans);
public:
	inline bool UseIndex() const;
};

inline bool Mesh::UseIndex() const{
	return indexNum > 0;
}