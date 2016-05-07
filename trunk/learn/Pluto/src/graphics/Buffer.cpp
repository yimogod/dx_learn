#include <graphics/Buffer.h>
using namespace plu;

Buffer::Buffer(int itemNum, int itemSize):
	GraphicsResource(itemNum, itemSize){
	_graphType = GT_BUFFER;
}

Buffer::~Buffer(){
}
