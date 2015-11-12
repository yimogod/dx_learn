#include "Mesh.h"
#include "Vector3D.h"
#include "Vertex.h"

Mesh::Mesh(){
	state = Mesh::STATE_ACTIVE;
}

Mesh::~Mesh(){}

void Mesh::setWorldPos(float x, float y, float z){
	position = Vector3D(x, y, z);
}

void Mesh::getVertexList(Vertex list[]){
	Vector3D vec;
	Color color;
	Vector3D normal;
	for(int i = 0; i < indexNum; i++){
		int index = indexList[i];
		vec = vertexList[index].add(position);
		color = vertexColorList[index];
		normal = vertexNormalList[index].add(position);

		list[i].pos = Float4{ vec.x, vec.y, vec.z, 1.0f};
		list[i].color = Float4A{ color.r, color.g, color.b, 1.0f};
		list[i].uv = Float2A{uvList[index* 2], uvList[index* 2 + 1]};
		list[i].normal = Float4{ normal.x, normal.y, normal.z, 1.0f};
	}
}

void Mesh::getVertexListV2(Vertex list[]){
	Vector3D vec;
	Color color;
	Vector3D normal;
	for(int i = 0; i < vertexNum; i++){
		vec = vertexList[i].add(position);
		color = vertexColorList[i];
		normal = vertexNormalList[i].add(position);

		list[i].pos = Float4{ vec.x, vec.y, vec.z, 1.0f };
		list[i].color = Float4A{ color.r, color.g, color.b, 1.0f };
		list[i].uv = Float2A{ uvList[i * 2], uvList[i * 2 + 1] };
		list[i].normal = Float4{ normal.x, normal.y, normal.z, 1.0f };
	}
}

void Mesh::calVertexNormal(){
	int triNum = (int)(indexNum / 3);
	for(int i = 0; i < triNum; i++){
		int i0 = indexList[i * 3 + 0];
		int i1 = indexList[i * 3 + 1];
		int i2 = indexList[i * 3 + 2];

		Vector3D v0 = vertexList[i0];
		Vector3D v1 = vertexList[i1];
		Vector3D v2 = vertexList[i2];

		Vector3D e0 = v1.sub(v0);
		Vector3D e1 = v2.sub(v0);
		Vector3D faceNormal = e0.cross(e1);

		vertexNormalList[i0].add(faceNormal);
		vertexNormalList[i1].add(faceNormal);
		vertexNormalList[i2].add(faceNormal);
	}

	for(int i = 0; i < vertexNum; i++){
		vertexNormalList[i].normalize();
	}
}