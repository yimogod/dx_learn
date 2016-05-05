#include <graphics/GraphicsObject.h>
using namespace plu;

GraphicsObject::GraphicsObject(){
	_graphType = GT_GRAPHICS_OBJECT;
}

GraphicsObject::~GraphicsObject(){
}

GraphicsObjectType GraphicsObject::getType(){
	return _graphType;
}

bool GraphicsObject::isBuffer(){
	return GT_BUFFER <= _graphType && _graphType <= GT_INDIRECT_ARGUMENTS_BUFFER;
}

bool GraphicsObject::isTexture(){
	return GT_TEXTURE_SINGLE <= _graphType && _graphType <= GT_TEXTURE3;
}

bool GraphicsObject::isTextureArray(){
	return GT_TEXTURE_ARRAY <= _graphType && _graphType <= GT_TEXTURE_CUBE_ARRAY;
}

bool GraphicsObject::isShader(){
	return GT_SHADER <= _graphType && _graphType <= GT_COMPUTE_SHADER;
}

bool GraphicsObject::isDrawingState(){
	return GT_DRAWING_STATE <= _graphType && _graphType <= GT_RASTERIZER_STATE;
}