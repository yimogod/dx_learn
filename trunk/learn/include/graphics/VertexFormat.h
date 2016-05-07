#pragma once
#include <graphics/DataFormat.h>

namespace plu{
	enum VASemantic{
		VA_NO_SEMANTIC, //û������
		VA_POSITION, //λ��
		VA_BLENDWEIGHT, 
		VA_BLENDINDICES,
		VA_NORMAL, //����
		VA_PSIZE, 
		VA_TEXCOORD, //��ͼ
		VA_TANGENT, //����
		VA_BINORMAL, //������
		VA_TESSFACTOR,
		VA_POSITIONT,
		VA_COLOR, //��ɫ
		VA_FOG, //��
		VA_DEPTH,
		VA_SAMPLE, //����
		VA_NUM_SEMANTICS
	};

	enum VAConstant{
		// The maximum number of attributes for a vertex format.
		VA_MAX_ATTRIBUTES = 16,

		// The maximum number of texture coordinate units.
		VA_MAX_TCOORD_UNITS = 8,

		// The maximum number of color units.
		VA_MAX_COLOR_UNITS = 2
	};

	class VertexFormat{
	public:
		VertexFormat();
		~VertexFormat();

		//���һ������, Ȼ������һ���������ԵĴ��������
		//TODO:unitʲô��˼? �ѵ���stride?
		bool bind(VASemantic semantic, DFType type, int unit);

		int getVertexSize();
		int getAttributesNum();
		//���µ�i������
		bool setAttribute(int index, VASemantic& semantic, DFType& type, int& unit, int& offset);

		//��ȡ�����λ��, ���û�з���-1
		int getIndex(VASemantic semantic, int unit);
		DFType getType(int index);
		int getOffset(int index);

	private:
		//�������Ը���
		int _attrNum;
		//�����ֽڴ�С, byte
		int _vertexSize;

	private:
		class Attribute{
		public:
			Attribute();
			VASemantic semantic;
			DFType type;
			int unit;
			int offset;
		};
		//���������б�, ���ڼ��㴫��dx layout������
		Attribute _attributes[VA_MAX_ATTRIBUTES];
	};
}