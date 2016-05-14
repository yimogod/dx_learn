#include <graphics/ConstantBuffer.h>
#include <algorithm>
using namespace plu;


ConstantBuffer::ConstantBuffer(int numBytes, bool allowDynamicUpdate)
    :Buffer(1, GetRoundedNumBytes(numBytes)){
    _graphType = GT_CONSTANT_BUFFER;
	_usage = (allowDynamicUpdate ? DYNAMIC_UPDATE : IMMUTABLE);
    memset(_rawData, 0, _totalBytes);
}

bool ConstantBuffer::HasMember(std::string& name){
	std::vector<MemberLayout>::iterator it;
	for(it = mLayout.begin(); it != mLayout.end(); it++){
		if((*it).name == name)return true;
	}

	return false;
}

int ConstantBuffer::GetRoundedNumBytes(int numBytes){
	if(numBytes <= 0)return 16;

    int remainder = numBytes % 16;
    if (remainder == 0)return numBytes;

    return numBytes + 16 - remainder;
}


std::vector<MemberLayout>& ConstantBuffer::GetLayout(){
	return mLayout;
}

template <typename T>
bool ConstantBuffer::SetMember(std::string& name, T& value){
	auto iter = std::find_if(mLayout.begin(), mLayout.end(),
		[&name](MemberLayout const& item){ return name == item.name; });

	if(iter == mLayout.end()){
		return false;
	}

	//此属性以及在buff里面, 用SetMember(name,index,value)进行替换值
	if(iter->numElements > 0){
		return false;
	}

	//const buff开辟的内存已被全部占用, 无法添加新的属性了
	if(iter->offset + sizeof(T) > mNumBytes){
		return false;
	}

	T* target = reinterpret_cast<T*>(_rawData + iter->offset);
	*target = value;
	return true;
}

template <typename T>
bool ConstantBuffer::GetMember(std::string& name, T& value){
	auto iter = std::find_if(mLayout.begin(), mLayout.end(),
		[&name](MemberLayout const& item){ return name == item.name; });

	//没找到这个属性
	if(iter == mLayout.end()){
		return false;
	}

	if(iter->numElements > 0){
		return false;
	}

	if(iter->offset + sizeof(T) > mNumBytes){
		return false;
	}

	T* target = reinterpret_cast<T*>(mData + iter->offset);
	value = *target;
	return true;
}

template <typename T>
bool ConstantBuffer::SetMember(std::string& name, int index, T& value){
	auto iter = std::find_if(mLayout.begin(), mLayout.end(),
		[&name](MemberLayout const& item){ return name == item.name; });

	if(iter == mLayout.end()){
		return false;
	}

	//Member is a singleton, use SetMember(name,value).
	if(iter->numElements == 0){
		return false;
	}

	//Index is out of range for the member array
	if(index >= iter->numElements){
		return false;
	}

	//LogError("Writing will access memory outside the buffer.");
	if(iter->offset + (index + 1)*sizeof(T) > mNumBytes){
		return false;
	}

	T* target = reinterpret_cast<T*>(mData + iter->offset + index*sizeof(T));
	*target = value;
	return true;
}

template <typename T>
bool ConstantBuffer::GetMember(std::string& name, int index, T& value){
	auto iter = std::find_if(mLayout.begin(), mLayout.end(),
		[&name](MemberLayout const& item){ return name == item.name; });

	//LogError("Failed to find member name " + name + ".");
	if(iter == mLayout.end()){
		return false;
	}

	//LogError("Member is a singleton, use GetMember(name,value).");
	if(iter->numElements == 0){
		return false;
	}

	//LogError("Index is out of range for the member array.");
	if(index >= iter->numElements){
		return false;
	}

	//LogError("Reading will access memory outside the buffer.");
	if(iter->offset + (index + 1)*sizeof(T) > mNumBytes){
		return false;
	}

	T* target = reinterpret_cast<T*>(mData + iter->offset + index*sizeof(T));
	value = *target;
	return true;
}
