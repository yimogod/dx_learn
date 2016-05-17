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

		VertexFormat& GetFormat();

		//��ȡ��ز����ҵ���Ӧͨ����ԭʼ�����е�ָ��
		char* GetChannel(VASemantic semantic, int unit, std::set<DFType>& requiredTypes);

	protected:
		VertexFormat _format;
	};

}