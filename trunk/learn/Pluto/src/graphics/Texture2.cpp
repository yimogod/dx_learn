#include <graphics/Texture2.h>
using namespace plu;


Texture2::Texture2(DFType format, int width, int height, bool hasMipmaps)
    :TextureSingle(format, 2, width, height, 1, hasMipmaps){
    _graphType = GT_TEXTURE2;
	mShared = false;
}

int Texture2::GetWidth(){
    return TextureSingle::GetDimension(0);
}

int Texture2::GetHeight(){
    return TextureSingle::GetDimension(1);
}

void Texture2::MakeShared(){
    // Shared textures are required to be GPU writable.
    _usage = SHADER_OUTPUT;
    mShared = true;
}

bool Texture2::IsShared(){
    return mShared;
}

