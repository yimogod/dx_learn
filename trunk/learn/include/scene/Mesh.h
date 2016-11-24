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

/* �ܶ����������ɵ������� */
class Mesh{
public:
    static const int STATE_ACTIVE = 1;
    static const int STATE_VISIBLE = 2;
    static const int STATE_CULLED = 4;
    
	Mesh();
	Mesh(const std::string path);
	~Mesh();
    
	DXVisual visual;
	//һ��material�ᱻ��ͬ��mesh����
	std::shared_ptr<Material> material = nullptr;

    std::string name;
	/* mesh��״̬ */
	int state;

	/*�������*/
	int vertexNum = 0;
	//���ܵ�������������
	Vertex* vertexList = nullptr;

	//λ������
	Vector3D posList[256];
	/*S, T, S, T, S, T, ��ͼ�����б�*/
	int uvNum = 0;// == vertexnum
	Vector2D uvList[256];
	/*����<��>����, �������ڶ�����*/
	Vector3D normalList[256];
	/*������������������*/
	Vector3D tangentList[256];
	

	//��������, ���=0, �����ǾͲ���drawindex��.
	int indexNum = 0;
	int indexList[512];
public:
	void InitStatic(Transform& trans);
	void Dispose();
	void UpdateCB(const void* data);//update const buffer

	/*���㷨������*/
	void CalVertexNormal();
	//��Ӷ���λ������. �������ҲҪ+1
	void AddVertexPos(Vector3D& value);
	void AddUVPos(Vector2D& value);
private:
	/*��ʽ������׼������vertext buff, ����ʹ��vbo*/
	/*length is vertex num*/
	void GetVertexList(Transform& trans);
public:
	inline bool UseIndex() const;
};

inline bool Mesh::UseIndex() const{
	return indexNum > 0;
}