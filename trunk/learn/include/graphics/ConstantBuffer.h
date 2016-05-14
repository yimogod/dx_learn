#pragma once
#include <vector>
#include <graphics/Buffer.h>
#include <graphics/MemberLayout.h>

namespace plu{
	class ConstantBuffer : public Buffer{
	public:
		//�����Ͼ���gl��uniform, ��������shader����, �Ǹ���̬����
		ConstantBuffer(int numBytes, bool allowDynamicUpdate);

		std::vector<MemberLayout>& GetLayout();

		//�Ƿ��б���name�������
		bool HasMember(std::string& name);

		// Set or get a non-array member.
		template <typename T>
		bool SetMember(std::string& name, T& value);

		template <typename T>
		bool GetMember(std::string& name, T& value);

		// Set or get an array member.
		template <typename T>
		bool SetMember(std::string& name, int index, T& value);

		template <typename T>
		bool GetMember(std::string& name, int index, T& value);

	protected:
		//���뵽16�ֽڵı���, HLSL��Ҫ
		static int GetRoundedNumBytes(int numBytes);

		// Allow the Shader class to set the data to avoid having to expose
		// setting the lookup table via a public interface.  The 'const' is
		// to allow Shader to call SetLayout from a 'const' ConstantBuffer.
		friend class Shader;
		mutable std::vector<MemberLayout> mLayout;

	public:
		// For use by the Shader class for storing reflection information.
		static int const shaderDataLookup = 0;
	};

}