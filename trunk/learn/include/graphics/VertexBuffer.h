#pragma once
#include <set>
#include <graphics/Buffer.h>
#include <graphics/VertexFormat.h>

namespace plu{
	class VertexBuffer : public Buffer{
	public:
		VertexBuffer(VertexFormat& format, int vertNum);
		VertexBuffer(int vertNum);
		~VertexBuffer();

		VertexFormat& getFormat();

		//获取相关参数找到对应通道在原始数据中的指针
		char* getChannel(VASemantic semantic, int unit, std::set<DFType>& requiredTypes);

	protected:
		VertexFormat _format;
	};

}