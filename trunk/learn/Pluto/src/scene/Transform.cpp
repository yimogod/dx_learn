#include <scene/Transform.h>

Transform::Transform(){
	dataType = TT_NULL;
}

Transform::~Transform(){
	dataType = TT_NULL;
}

void Transform::SetParent(std::shared_ptr<Transform> parent){
	if(_parent != nullptr)
		_parent->RemoveChild(std::shared_ptr<Transform>(this));

	_parent = parent;
	_parent->AddChild(std::shared_ptr<Transform>(this));
}

int Transform::AddChild(std::shared_ptr<Transform> const& child){
	child->SetParent(std::shared_ptr<Transform>(this));

	// Insert the child in the first available slot (if any).
	int i = 0;
	for(auto& current : _children){
		if(!current){
			current = child;
			return i;
		}
		++i;
	}

	// All slots are used, so append the child to the array.
	int const numChildren = static_cast<int>(_children.size());
	_children.push_back(child);
	return numChildren;
}

std::shared_ptr<Transform> Transform::GetChild(int index){
	if(index < 0)return nullptr;
	if(index >= static_cast<int>(_children.size()))return nullptr;

	return _children[index];
}

void Transform::RemoveAllChildren(){
	for(auto& current : _children){
		RemoveChild(current);
	}
}

int Transform::RemoveChild(std::shared_ptr<Transform> const& child){
	if(child == nullptr)return -1;

	int i = 0;
	for(auto& current : _children){
		if(current == child){
			current->SetParent(nullptr);
			current = nullptr;
			return i;
		}
		++i;
	}

	return -1;
}

std::shared_ptr<Transform> Transform::RemoveChildAt(int index){
	if(index < 0)return nullptr;
	if(index >= static_cast<int>(_children.size()))return nullptr;

	std::shared_ptr<Transform> child = _children[index];
	if(child){
		child->SetParent(nullptr);
		_children[index] = nullptr;
	}
	return child;
}

void Transform::Translate(Vector3D& src){
	Matrix4x4 model = localToWorldMatrix().transpose();
	float x = src.x;
	float y = src.y;
	float z = src.z;

	src.x = x * model.m00 + y * model.m10 + z * model.m20;
	src.y = x * model.m01 + y * model.m11 + z * model.m21;
	src.z = x * model.m02 + y * model.m12 + z * model.m22;
}

Matrix4x4 Transform::localToWorldMatrix(){
	Matrix4x4 mat(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x, position.y, position.z, 1.0f);
	Matrix4x4 m = mat.RotateY(eulerAngle.y);
	m = m.RotateX(eulerAngle.x);
	m = m.rotateZ(eulerAngle.z);

	return m;
}

void Transform::RotateX(float x){
	eulerAngle.x += x;
}
void Transform::RotateY(float y){
	eulerAngle.y += y;
}

void Transform::SetWorldPos(float x, float y, float z){
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::SetScale(float value){
	scale.x = value;
	scale.y = value;
	scale.z = value;
}

void Transform::Move(float x, float y, float z){
	position.x += x;
	position.y += y;
	position.z += z;
}

void Transform::SetMesh(Mesh* mesh){
	dataType = TT_Mesh;
	data = mesh;
}
