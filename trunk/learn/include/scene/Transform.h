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
	//-1��Ϊ�ⲻ�ɹ�
	int AddChild(std::shared_ptr<Transform> const& child);
	//ע��!!�����ȥ�����Ӳ�û������������TODO
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

