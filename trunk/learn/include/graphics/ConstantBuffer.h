#pragma once
#include <vector>
#include <graphics/Buffer.h>
#include <graphics/MemberLayout.h>

namespace plu{
	class ConstantBuffer : public Buffer{
	public:
		//基本上就是gl的uniform, 对于整个shader而言, 是个静态变量
		ConstantBuffer(int numBytes, bool allowDynamicUpdate);

		std::vector<MemberLayout>& GetLayout();

		//是否有变量name这个属性
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
		//补齐到16字节的倍数, HLSL需要
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