#include <math/algebra/Vector3D.h>
#include <graphics/Mesh.h>
#include <graphics/Vertex.h>

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
	Vector2D uv;
	for(int i = 0; i < indexNum; i++){
		int index = indexList[i];
		normal = vertexNormalList[i];
		int uvIndex = uvIndexList[i];
		vec = vertexList[index].add(position);
		color = vertexColorList[index];
		uv = uvList[uvIndex];

		list[i].pos = Float4{vec.x, vec.y, vec.z, 1.0f};
		list[i].color = Float4A{color.r, color.g, color.b, 1.0f};
		list[i].uv = Float2A{uv.x, uv.y};
		list[i].normal = Float4{normal.x, normal.y, normal.z, 1.0f};
	}
}

void Mesh::getVertexPosList(Vertex list[]){
	Vector3D vec;
	Color color;
	for(int i = 0; i < indexNum; i++){
		int index = indexList[i];
		vec = vertexList[index].add(position);
		color = vertexColorList[index];

		list[i].pos = Float4{ vec.x, vec.y, vec.z, 1.0f };
		list[i].color = Float4A{ color.r, color.g, color.b, 1.0f };
	}
}

void Mesh::getVertexUVList(Vertex list[]){
	Vector2D uv;
	for(int i = 0; i < indexNum; i++){
		int uvIndex = uvIndexList[i];
		uv = uvList[uvIndex];

		list[i].uv = Float2A{ uv.x, uv.y };
	}
}

void Mesh::getVertexNormalList(Vertex list[]){
	Vector3D normal;
	for(int i = 0; i < indexNum; i++){
		normal = vertexNormalList[i];
		list[i].normal = Float4{ normal.x, normal.y, normal.z, 1.0f };
	}
}

void Mesh::getVertexList_v2(Vertex list[]){
	Vector3D vec;
	Color color;
	Vector3D normal;
	for(int i = 0; i < vertexNum; i++){
		vec = vertexList[i].add(position);
		color = vertexColorList[i];
		normal = vertexNormalList[i].add(position);

		list[i].pos = Float4{ vec.x, vec.y, vec.z, 1.0f };
		list[i].color = Float4A{ color.r, color.g, color.b, 1.0f };
		list[i].uv = Float2A{ uvList[i].x, uvList[i].y };
		list[i].normal = Float4{ normal.x, normal.y, normal.z, 1.0f };
	}
}

/* 这个版本的法线是基于index的. 及36个index会有36个点
 * 这是之前的版本的问题--顶点顺序, 逆时针排列, 这里法线的个数跟定点数相等,
 * 意味着顶点法线是连接面法线的平均值. 这样在平面内进行插值的话会有错误.
 * 因为实际上, 插值的应该是在顶点本平面内的法线进行插值
 */
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
		faceNormal.normalize();

		vertexNormalList[i * 3 + 0] = faceNormal;
		vertexNormalList[i * 3 + 1] = faceNormal;
		vertexNormalList[i * 3 + 2] = faceNormal;
	}
}