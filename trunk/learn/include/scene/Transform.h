#pragma once
#include <vector>
#include <memory>
#include <scene/Mesh.h>

class Transform
{
public:
	Transform();
	~Transform();

	enum TransformType{
		TT_NULL = 1UL << 0,
		TT_Mesh = 1UL << 1,
		TT_Camera = 1UL << 2,
		TT_Light = 1UL << 3,
	};

	TransformType dataType;

	//树相关
	inline std::shared_ptr<Transform> GetParent();
	inline int GetNumChildren() const;

	void SetParent(std::shared_ptr<Transform> parent);
	//-1以为这不成功
	int AddChild(std::shared_ptr<Transform> const& child);
	//注意!!这里的去除孩子并没有重新排序孩子TODO
	int RemoveChild(std::shared_ptr<Transform> const& child);
	std::shared_ptr<Transform> RemoveChildAt(int i);
	void RemoveAllChildren();

	std::shared_ptr<Transform> GetChild(int index);

	//数据相关,设置data为mesh
	void SetMesh(Mesh* mesh);
	template<typename T>
	inline T GetData();

	//变换相关
	/* mesh在世界中的坐标 */
	Vector3D position;
	Vector3D eulerAngle;
	Vector3D scale;

	void SetWorldPos(float x, float y, float z);
	void SetScale(float scale);
	void Move(float x, float y, float z);
	/* mesh在世界中的旋转, 旋转值是弧度 */
	void RotateX(float x);
	void RotateY(float y);
	Matrix4x4 localToWorldMatrix();
private:
	void* data = nullptr;
	std::shared_ptr<Transform> _parent;
	std::vector<std::shared_ptr<Transform>> _children;
};

inline std::shared_ptr<Transform> Transform::GetParent(){
	return _parent;
}

int Transform::GetNumChildren() const{
	return static_cast<int>(_children.size());
}

template<typename T>
inline T Transform::GetData(){
	return static_cast<T>(data);
}