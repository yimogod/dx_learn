#include <graphics/IndexBuffer.h>
using namespace plu;

IndexBuffer::IndexBuffer(IPType type, int numPrimitives, int indexSize)
    :Buffer(numPrimitives, indexSize){
    _graphType = GT_INDEX_BUFFER;

	mPrimitiveType = type;
	mNumPrimitives = numPrimitives;
}

IndexBuffer::IndexBuffer(IPType type, unsigned int numPrimitives)
	:Buffer(numPrimitives, 0){
	_graphType = GT_INDEX_BUFFER;

	mPrimitiveType = type;
	mNumPrimitives = numPrimitives;
}

IPType IndexBuffer::GetPrimitiveType() const{
	return mPrimitiveType;
}

int IndexBuffer::GetNumPrimitives() const{
	return mNumPrimitives;
}

bool IndexBuffer::IsIndexed() const{
	return _rawData != nullptr;
}

int IndexBuffer::GetIndexCout() const{
	return mNumPrimitives > 0 ? 3 * mNumPrimitives : 3;
}

bool IndexBuffer::ValidPrimitiveType(IPType type) const{
	if((mPrimitiveType & type) != 0)return true;
	return false;
}

bool IndexBuffer::GetTriangle(int i, int& v0, int& v1, int& v2) const {
	if(!ValidPrimitiveType(IP_TRIMESH))return false;
	if(_rawData == nullptr)return false;
	if(i >= mNumPrimitives)return false;

	//索引都是int, 如果不是int那就是高端应用了
    if (_itemSize == sizeof(int)){
        int ii = i * 3;
        //v0 = index[ii + 0];
        //v1 = index[ii + 1];
        //v2 = index[ii + 2];
		return true;
    }
           
    return false;
}