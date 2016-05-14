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
		int GetIndexCout() const;

		//��ȡ��i�������ε��������������
		bool GetTriangle(int i, int& v0, int& v1, int& v2) const;

	protected:
		//ͼԪ��������
		IPType mPrimitiveType;
		int mNumPrimitives;

		//��index buffer�Ƿ���type����
		bool ValidPrimitiveType(IPType type) const;
	};
}
