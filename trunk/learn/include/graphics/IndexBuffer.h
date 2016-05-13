#pragma once
#include <graphics/Buffer.h>
#include <graphics/IndexFormat.h>

namespace plu{
	class IndexBuffer : public Buffer{
	public:
		IndexBuffer(IPType type, int numPrimitives, int indexSize);
		IndexBuffer(IPType type, unsigned int numPrimitives);

		IPType GetPrimitiveType() const;
		int GetNumPrimitives() const;
		bool IsIndexed() const;

		//获取第i个三角形的三个顶点的索引
		bool GetTriangle(int i, int& v0, int& v1, int& v2) const;

	protected:
		//图元排列类型
		IPType mPrimitiveType;
		int mNumPrimitives;

		//但index buffer是否是type类型
		bool ValidPrimitiveType(IPType type) const;

		typedef unsigned int(*ICFunction)(unsigned int);
		static ICFunction msIndexCounter[IP_NUM_TYPES];

		static unsigned int GetPolypointIndexCount(
			unsigned int numPrimitives);

		static unsigned int GetPolysegmentDisjointIndexCount(
			unsigned int numPrimitives);

		static unsigned int GetPolysegmentContiguousIndexCount(
			unsigned int numPrimitives);

		static unsigned int GetTrimeshIndexCount(
			unsigned int numPrimitives);

		static unsigned int GetTristripIndexCount(
			unsigned int numPrimitives);
	};
}
