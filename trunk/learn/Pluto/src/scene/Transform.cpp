#include <scene/Transform.h>

Transform::Transform()
{
}

Transform::~Transform()
{
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