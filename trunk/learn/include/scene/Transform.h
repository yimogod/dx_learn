#pragma once
#include<vector>
#include<memory>

class Transform
{
public:
	Transform();
	~Transform();

	inline Transform* GetParent();
	inline int GetNumChildren() const;

	void SetParent(Transform* parent);
	//-1以为这不成功
	int AddChild(std::shared_ptr<Transform> const& child);
	//注意!!这里的去除孩子并没有重新排序孩子TODO
	int RemoveChild(std::shared_ptr<Transform> const& child);
	std::shared_ptr<Transform> RemoveChildAt(int i);
	void RemoveAllChildren();

	std::shared_ptr<Transform> GetChild(int index);
private:
	Transform* _parent;
	std::vector<std::shared_ptr<Transform>> _children;
};

inline Transform* Transform::GetParent(){
	return _parent;
}

int Transform::GetNumChildren() const{
	return static_cast<int>(_children.size());
}

