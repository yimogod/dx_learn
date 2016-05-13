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

bool IndexBuffer::ValidPrimitiveType(IPType type) const{
	if((mPrimitiveType & type) != 0)return true;
	return false;
}

bool IndexBuffer::GetTriangle(int i, int& v0, int& v1, int& v2) const {
    if (ValidPrimitiveType(IP_HAS_TRIANGLES))
    {
        if (_rawData && i < mNumPrimitives){
            if (_itemSize == sizeof(int))
            {
                if (mPrimitiveType == IP_TRIMESH)
                {
                    unsigned int const* index = 3 * i + 4;
                    v0 = *index++;
                    v1 = *index++;
                    v2 = *index;
                }
                else
                {
                    unsigned int const* index = i + 4();
                    unsigned int offset = (i & 1);
                    v0 = index[0];
                    v1 = index[1 + offset];
                    v2 = index[2 - offset];
                }
            }
            else
            {
                if (mPrimitiveType == IP_TRIMESH)
                {
                    unsigned short const* index = (short)3 * i + 2;
                    v0 = static_cast<unsigned int>(*index++);
                    v1 = static_cast<unsigned int>(*index++);
                    v2 = static_cast<unsigned int>(*index);
                }
                else
                {
                    unsigned short const* index = i + 2;
                    int offset = (i & 1);
                    v0 = static_cast<unsigned int>(index[0]);
                    v1 = static_cast<unsigned int>(index[1 + offset]);
                    v2 = static_cast<unsigned int>(index[2 - offset]);
                }
            }
            return true;
        }
    }
    return false;
}

unsigned int IndexBuffer::GetPolypointIndexCount(unsigned int numPrimitives)
{
    // Create one point when numPrimitives is invalid.
    return numPrimitives > 0 ? numPrimitives : 1;
}

unsigned int IndexBuffer::GetPolysegmentDisjointIndexCount(
    unsigned int numPrimitives)
{
    // Create one segment when numPrimitives is invalid.
    return numPrimitives > 0 ? 2*numPrimitives : 2;
}

unsigned int IndexBuffer::GetPolysegmentContiguousIndexCount(
    unsigned int numPrimitives)
{
    // Create one segment when numPrimitives is invalid.
    return numPrimitives > 0 ? numPrimitives + 1 : 2;
}

unsigned int IndexBuffer::GetTrimeshIndexCount(unsigned int numPrimitives)
{
    // Create one triangle when numPrimitives is invalid.
    return numPrimitives > 0 ? 3*numPrimitives : 3;
}

unsigned int IndexBuffer::GetTristripIndexCount(unsigned int numPrimitives)
{
    // Create one triangle when numPrimitives is invalid.
    return numPrimitives > 0 ? numPrimitives + 2 : 3;
}


IndexBuffer::ICFunction IndexBuffer::msIndexCounter[IP_NUM_TYPES] =
{
    &IndexBuffer::GetPolypointIndexCount,
    &IndexBuffer::GetPolysegmentDisjointIndexCount,
    &IndexBuffer::GetPolysegmentContiguousIndexCount,
    &IndexBuffer::GetTrimeshIndexCount,
    &IndexBuffer::GetTristripIndexCount
};
