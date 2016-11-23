#include <graphics/DXEngine.h>
#include <math/algebra/Vector3D.h>
#include <scene/Mesh.h>
#include <util/CUtil.h>

Mesh::Mesh(){
	state = Mesh::STATE_ACTIVE;
}

Mesh::~Mesh(){}

void Mesh::Init(){
	if(material == nullptr){
		std::cout << "material is null. set a value before init the mesh" << std::endl;
		return;
	}


	/*准备shader数据*/
	std::wstring shaderName = material->GetName_w();
	visual.PreInitShader(shaderName.c_str(), shaderName.c_str());


	//创建 layout, 如果之前我们没有手动创建layout, 那我们就用标准的渲染模型的layout
	//目前只有instance实例会手动指定
	if(visual.GetLayoutNum() == 0)
		visual.PreAddDefaultLayout();

	//创建buffer需要的变量
	//如果我们没有手动添加多个constbuffer, 那我们就默认添加mvp
	//这些我们应该从material读取--TODO
	if(visual.GetConstBufferNum() == 0)
		visual.PreSetVSConstBufferSize(sizeof(MVPConstBuffer));

	vertexList = new Vertex[vertexNum];
	GetVertexList();

	visual.Init(DXEngine::Instance().GetDevice(),
		(char*)vertexList, vertexNum, indexList, indexNum);
}

void Mesh::SetWorldPos(float x, float y, float z){
	position.x = x;
	position.y = y;
	position.z = z;
}

void Mesh::SetScale(float scale){
	for(int i = 0; i < vertexNum; i++){
		Vector3D vec = posList[i];
		posList[i] = vec.mul(scale);
	}
}

void Mesh::Move(float x, float y, float z){
	position.x += x;
	position.y += y;
	position.z += z;
}


void Mesh::GetVertexList(){
	for(int i = 0; i < vertexNum; i++){
		Vector3D vec = posList[i];
		Vector3D normal = normalList[i];
		Vector3D tangent = tangentList[i];

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

/* 这个版本的法线是基于index的. 及36个index会有36个点
 * 这是之前的版本的问题--顶点顺序, 逆时针排列, 这里法线的个数跟定点数相等,
 * 意味着顶点法线是连接面法线的平均值. 这样在平面内进行插值的话会有错误.
 * 因为实际上, 插值的应该是在顶点本平面内的法线进行插值
 */
void Mesh::CalVertexNormal(){
	bool b = UseIndex();
	int triNum = 0;
	
	if(b){//使用的是索引模式
		triNum = (int)(indexNum / 3);
	}else{ //使用的是顶点模式
		triNum = (int)(vertexNum / 3);
	}

	int i0, i1, i2;
	for(int i = 0; i < triNum; i++){
		//由定点坐标计算法线
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
		//计算点法线
		Vector3D e0 = v1.sub(v0);
		Vector3D e1 = v2.sub(v0);
		Vector3D faceNormal = e0.cross(e1);
		faceNormal.normalize();

		normalList[i0] = normalList[i0].add(faceNormal);
		normalList[i1] = normalList[i1].add(faceNormal);
		normalList[i2] = normalList[i2].add(faceNormal);

		//3个uv坐标, 由uv坐标计算点切线
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

void Mesh::RotateX(float x){
	eulerAngle.x += x;
}
void Mesh::RotateY(float y){
	eulerAngle.y += y;
}

Matrix4x4 Mesh::localToWorldMatrix(){
	Matrix4x4 mat(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x, position.y, position.z, 1.0f);
	Matrix4x4 m = mat.RotateY(eulerAngle.y);
	m = m.RotateX(eulerAngle.x);
	m = m.rotateZ(eulerAngle.z);

	return m;
}

void Mesh::Dispose(){
	if(vertexList != nullptr)delete(vertexList);
	vertexList = nullptr;
}