#include <graphics/GraphicsResource.h>
using namespace plu;

GraphicsResource::GraphicsResource(int itemNum, int itemSize){
	_itemNum = itemNum;
	_itemSize = itemSize;
	_usage = IMMUTABLE;
	_offset = 0;

	_graphType = GT_RESOURCE;

	_totalBytes = _itemNum * _itemSize;
}


GraphicsResource::~GraphicsResource(){
}

void GraphicsResource::SetCopyType(CopyType copyType){
	mCopyType = copyType;
}

GraphicsResource::CopyType GraphicsResource::GetCopyType(){
	return mCopyType;
}

void GraphicsResource::ResetData(){
	_rawData = nullptr;
}

void GraphicsResource::SetData(char* data){
	_rawData = data;
}

char* GraphicsResource::GetData(){
	return _rawData;
}