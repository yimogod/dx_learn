#include <graphics/TextureSingle.h>
using namespace plu;

TextureSingle::TextureSingle(DFType format, int numDimensions,
    int dim0, int dim1, int dim2, bool hasMipmaps)
	:Texture(1, format, numDimensions, dim0, dim1, dim2, hasMipmaps){
    _graphType = GT_TEXTURE_SINGLE;
}


int TextureSingle::GetOffsetFor(int level){
	return Texture::GetOffsetFor(0, level);
}

char* TextureSingle::GetDataFor(int level){
	return Texture::GetDataFor(0, level);
}

template <typename T>
T* TextureSingle::GetFor(int level){
	return Texture::GetFor<T>(0, level);
}