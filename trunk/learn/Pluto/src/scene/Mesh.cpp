#include <graphics/DXEngine.h>
#include <math/algebra/Vector3D.h>
#include <scene/Mesh.h>
#include <util/CUtil.h>
#include <util/ObjParser.h>

Mesh::Mesh(){
	state = Mesh::STATE_ACTIVE;
}

Mesh::Mesh(const std::string path){
	state = Mesh::STATE_ACTIVE;

	ObjParser reader;
	reader.Read(this, path.c_str());
}

Mesh::~Mesh(){
	state = Mesh::STATE_ACTIVE;
	material = nullptr;
	vertexList = nullptr;
}

void Mesh::InitStatic(Transform& trans){
	if(material == nullptr){
		std::cout << "material is null. set a value before init the mesh" << std::endl;
		return;
	}


	/*׼��shader����*/
	std::wstring shaderName = material->GetName_w();
	visual.PreInitShader(shaderName.c_str(), shaderName.c_str());

	int textNum = material->GetTextureNum();
	for(int i = 0; i < textNum; i++){
		Texture2D* texture = material->GetTexture(i);
		visual.PreAddTexture(texture->GetPath());
	}

	//���� layout, ���֮ǰ����û���ֶ�����layout, �����Ǿ��ñ�׼����Ⱦģ�͵�layout
	//Ŀǰֻ��instanceʵ�����ֶ�ָ��
	if(visual.GetLayoutNum() == 0)
		visual.PreAddDefaultLayout();

	for(int i = 0; i < material->Get_VS_CBufferNum(); i++){
		int size = material->Get_VS_CBufferSize(i);
		visual.PreSetVSConstBufferSize(size);
	}

	for(int i = 0; i < material->Get_PS_CBufferNum(); i++){
		int size = material->Get_PS_CBufferSize(i);
		visual.PreSetPSConstBufferSize(size);
	}

	vertexList = new Vertex[vertexNum];
	GetVertexList(trans);

	visual.Init(DXEngine::Instance().GetDevice(),
		(char*)vertexList, vertexNum, indexList, indexNum);
}

void Mesh::UpdateCB(const void* data){
	visual.UpdateVSConstBuffer(DXEngine::Instance().GetContext(), 0, data);
}

void Mesh::GetVertexList(Transform& trans){
	for(int i = 0; i < vertexNum; i++){
		Vector3D vec = posList[i];
		Vector3D normal = normalList[i];
		Vector3D tangent = tangentList[i];
		trans.Translate(vec);
		trans.Translate(normal);
		trans.Translate(tangent);

		vertexList[i].pos = Float4{ vec.x, vec.y, vec.z, 1.0f };
		vertexList[i].uv = Float2A{ uvList[i].x, uvList[i].y };
		vertexList[i].normal = Float4{ normal.x, normal.y, normal.z, 1.0f };
		vertexList[i].tangent = Float4{ tangent.x, tangent.y, tangent.z, 1.0f };
	}
}

void Mesh::AddVertexPos(Vector3D& value){
	posList[vertexNum] = value;
	vertexNum++;
}

void Mesh::AddUVPos(Vector2D& value){
	uvList[uvNum] = value;
	uvNum++;
}

/* ����汾�ķ����ǻ���index��. ��36��index����36����
 * ����֮ǰ�İ汾������--����˳��, ��ʱ������, ���﷨�ߵĸ��������������,
 * ��ζ�Ŷ��㷨���������淨�ߵ�ƽ��ֵ. ������ƽ���ڽ��в�ֵ�Ļ����д���.
 * ��Ϊʵ����, ��ֵ��Ӧ�����ڶ��㱾ƽ���ڵķ��߽��в�ֵ
 */
void Mesh::CalVertexNormal(){
	bool b = UseIndex();
	int triNum = 0;
	
	if(b){//ʹ�õ�������ģʽ
		triNum = (int)(indexNum / 3);
	}else{ //ʹ�õ��Ƕ���ģʽ
		triNum = (int)(vertexNum / 3);
	}

	int i0, i1, i2;
	for(int i = 0; i < triNum; i++){
		//�ɶ���������㷨��
		if(b){
			i0 = indexList[i * 3 + 0];
			i1 = indexList[i * 3 + 1];
			i2 = indexList[i * 3 + 2];
		}else{
			i0 = i * 3 + 0;
			i1 = i * 3 + 1;
			i2 = i * 3 + 2;
		}

		Vector3D v0 = posList[i0];
		Vector3D v1 = posList[i1];
		Vector3D v2 = posList[i2];
		//����㷨��
		Vector3D e0 = v1.sub(v0);
		Vector3D e1 = v2.sub(v0);
		Vector3D faceNormal = e0.cross(e1);
		faceNormal.normalize();

		normalList[i0] = normalList[i0].add(faceNormal);
		normalList[i1] = normalList[i1].add(faceNormal);
		normalList[i2] = normalList[i2].add(faceNormal);

		//3��uv����, ��uv������������
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

		tangentList[i0] = tangentList[i0].add(tangent);
		tangentList[i1] = tangentList[i1].add(tangent);
		tangentList[i2] = tangentList[i2].add(tangent);

		/*Vector3D binormal;
		binormal.x = (u0.x * e1.x - u0.y * e0.x) * den;
		binormal.x = (u0.x * e1.y - u0.y * e0.y) * den;
		binormal.x = (u0.x * e1.z - u0.y * e0.z) * den;
		binormal.normalize();

		Vector3D a = tangent.cross(binormal);*/
	}
}

void Mesh::Dispose(){
	if(vertexList != nullptr)delete(vertexList);
	vertexList = nullptr;
}