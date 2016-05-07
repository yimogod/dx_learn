#include <graphics/VertexBuffer.h>
using namespace plu;

VertexBuffer::VertexBuffer(VertexFormat& format, int vertNum):
	Buffer(vertNum, format.getVertexSize()){
	_format = format;
	_graphType = GT_VERTEX_BUFFER;
}
VertexBuffer::VertexBuffer(int vertNum):
	Buffer(vertNum, 0){
	_graphType = GT_VERTEX_BUFFER;
}

VertexBuffer::~VertexBuffer(){
}

VertexFormat& VertexBuffer::getFormat(){
	return _format;
}

char* VertexBuffer::getChannel(VASemantic semantic, int unit, std::set<DFType>& requiredTypes){
	if(!_rawData)return nullptr;

	int index = _format.getIndex(semantic, unit);
	if(index < 0)return nullptr;

	DFType type = _format.getType(index);
	if(requiredTypes.size() > 0){
		if(requiredTypes.find(type) == requiredTypes.end()){
			return nullptr;
		}
	}

	return _rawData + _format.getOffset(index);
}
