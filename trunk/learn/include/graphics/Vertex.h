#pragma once
#include <BaseDataStruct.h>

struct Vertex{
	Float4 pos;
	Float4A color;
	Float2A uv;
	Float4 normal;//如果使用法线贴图, 那么normal就不是面法线了. 而是切线空间的法线轴, 但理论上跟面法线的值是一样的
	Float4 tangent;
};