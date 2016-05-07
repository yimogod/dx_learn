#pragma once
#include <graphics/DataFormat.h>

namespace plu{
	enum VASemantic{
		VA_NO_SEMANTIC, //没有语意
		VA_POSITION, //位置
		VA_BLENDWEIGHT, 
		VA_BLENDINDICES,
		VA_NORMAL, //法线
		VA_PSIZE, 
		VA_TEXCOORD, //贴图
		VA_TANGENT, //切线
		VA_BINORMAL, //副法线
		VA_TESSFACTOR,
		VA_POSITIONT,
		VA_COLOR, //颜色
		VA_FOG, //雾
		VA_DEPTH,
		VA_SAMPLE, //采样
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

		//添加一条数据, 然后生成一个顶点属性的打包的数据
		//TODO:unit什么意思? 难道是stride?
		bool bind(VASemantic semantic, DFType type, int unit);

		int getVertexSize();
		int getAttributesNum();
		//更新第i个属性
		bool setAttribute(int index, VASemantic& semantic, DFType& type, int& unit, int& offset);

		//获取语意的位置, 如果没有返回-1
		int getIndex(VASemantic semantic, int unit);
		DFType getType(int index);
		int getOffset(int index);

	private:
		//顶点属性个数
		int _attrNum;
		//顶点字节大小, byte
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
		//顶点属性列表, 用于计算传给dx layout的数据
		Attribute _attributes[VA_MAX_ATTRIBUTES];
	};
}