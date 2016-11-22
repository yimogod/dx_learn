#pragma once
#include<vector>
#include<memory>

class Transform
{
public:
	Transform();
	~Transform();

	enum TransformType
	{
		TT_NULL = 1UL << 0,
		TT_Mesh = 1UL << 1,
		TT_Camera = 1UL << 2,
		TT_Light = 1UL << 3,
	};

	TransformType dataType;
	void* data = nullptr;

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

	template<typename T>
	inline T GetData();
private:
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