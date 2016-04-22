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
		normal = normalList[i];
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
		normal = normalList[i];
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
		normal = normalList[i].add(position);

		list[i].pos = Float4{ vec.x, vec.y, vec.z, 1.0f };
		list[i].color = Float4A{ color.r, color.g, color.b, 1.0f };
		list[i].uv = Float2A{ uvList[i].x, uvList[i].y };
		list[i].normal = Float4{ normal.x, normal.y, normal.z, 1.0f };
	}
}

/* ����汾�ķ����ǻ���index��. ��36��index����36����
 * ����֮ǰ�İ汾������--����˳��, ��ʱ������, ���﷨�ߵĸ��������������,
 * ��ζ�Ŷ��㷨���������淨�ߵ�ƽ��ֵ. ������ƽ���ڽ��в�ֵ�Ļ����д���.
 * ��Ϊʵ����, ��ֵ��Ӧ�����ڶ��㱾ƽ���ڵķ��߽��в�ֵ
 */
void Mesh::calVertexNormal(){
	int triNum = (int)(indexNum / 3);
	for(int i = 0; i < triNum; i++){
		//�ɶ���������㷨��
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

		normalList[i * 3 + 0] = faceNormal;
		normalList[i * 3 + 1] = faceNormal;
		normalList[i * 3 + 2] = faceNormal;

		//��uv�����������
		i0 = uvIndexList[i * 3 + 0];
		i1 = uvIndexList[i * 3 + 1];
		i2 = uvIndexList[i * 3 + 2];
		
		Vector2D t0 = uvList[i0];
		Vector2D t1 = uvList[i1];
		Vector2D t2 = uvList[i2];
		
		Vector2D u0 = t1.sub(t0);
		Vector2D u1 = t2.sub(t0);

		Vector3D tangent;
		float den = 1.0f / (u0.x * u1.y - u0.y * u1.x);
		tangent.x = (u1.y * e0.x - u1.x * e1.x) * den;
		tangent.y = (u1.y * e0.y - u1.x * e1.y) * den;
		tangent.z = (u1.y * e0.z - u1.x * e1.z) * den;
		tangent.normalize();
		tangent = e0.div(u0.x);
		tangentList[i * 3 + 0] = tangent;
		tangentList[i * 3 + 1] = tangent;
		tangentList[i * 3 + 2] = tangent;

		/*Vector3D binormal;
		binormal.x = (u0.x * e1.x - u0.y * e0.x) * den;
		binormal.x = (u0.x * e1.y - u0.y * e0.y) * den;
		binormal.x = (u0.x * e1.z - u0.y * e0.z) * den;
		binormal.normalize();

		Vector3D a = tangent.cross(binormal);*/
	}
}

void Mesh::rotateX(float x){
	eulerAngle.x += x;
}
void Mesh::rotateY(float y){
	eulerAngle.y += y;
}

Matrix4x4 Mesh::localToWorldMatrix(){
	Matrix4x4 mat(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-position.x, -position.y, -position.z, 1.0f);
	Matrix4x4 m = mat.rotateY(eulerAngle.y);
	m = m.rotateX(eulerAngle.x);
	m = m.rotateZ(eulerAngle.z);

	return m;
}