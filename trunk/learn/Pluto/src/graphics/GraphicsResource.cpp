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
