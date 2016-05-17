#include <graphics/VertexFormat.h>
using namespace plu;

VertexFormat::VertexFormat(){
	_attrNum = 0;
	_vertexSize = 0;
}

bool VertexFormat::bind(VASemantic semantic, DFType type, int unit){
	if(0 <= _attrNum && _attrNum < VA_MAX_ATTRIBUTES){
		//验证输入的数据是否合法
		if(semantic == VA_COLOR && unit >= VA_MAX_COLOR_UNITS){
			return false;
		}

		if(semantic == VA_TEXCOORD && unit >= VA_MAX_TCOORD_UNITS){
			return false;
		}

		// Accept the attribute.
		Attribute& attribute = _attributes[_attrNum];
		attribute.semantic = semantic;
		attribute.type = type;
		attribute.unit = unit;
		attribute.offset = _vertexSize;
		++_attrNum;

		// 重新计算顶点字节
		_vertexSize += DataFormat::GetNumBytesPerStruct(type);
		return true;
	}

	return false;
}

int VertexFormat::getVertexSize(){
	return _vertexSize;
}

int VertexFormat::getAttributesNum(){
	return _attrNum;
}

bool VertexFormat::setAttribute(int index, VASemantic& semantic, DFType& type, int& unit, int& offset){
	if(0 <= index && index < _attrNum){
		Attribute const& attribute = _attributes[index];
		semantic = attribute.semantic;
		type = attribute.type;
		unit = attribute.unit;
		offset = attribute.offset;
		return true;
	}

	return false;
}

int VertexFormat::getIndex(VASemantic semantic, int unit){
	for(int i = 0; i < _attrNum; ++i){
		Attribute const& attribute = _attributes[i];
		if(attribute.semantic == semantic && attribute.unit == unit){
			return i;
		}
	}

	return -1;
}

DFType VertexFormat::getType(int i){
	if(0 <= i && i < _attrNum){
		return _attributes[i].type;
	}

	return DF_UNKNOWN;
}

int VertexFormat::getOffset(int i){
	if(0 <= i && i < _attrNum){
		return _attributes[i].offset;
	}

	return 0xFFFFFFFFu;
}

VertexFormat::Attribute::Attribute(){
	semantic = VA_NO_SEMANTIC;
	type = DF_UNKNOWN;
	unit = 0;
	offset = 0;
}

