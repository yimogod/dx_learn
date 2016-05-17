#pragma once

#include <graphics/Buffer.h>
#include <graphics/MemberLayout.h>
#include <graphics/Texture.h>

namespace plu{
	class TextureBuffer : public Buffer{
	public:
		// Construction.  The HLSL shader contains
		//   tbuffer MyTBuffer { type myArray[numElements]; }
		// where 'type' is a native type such as 'float4' and the texture buffer
		// 'format' specifies how the type is interpreted.  The 'numElements' of
		// the constructor must match that of myArray[].  Effectively, the
		// tbuffer is a 1D texture.  If you want to update the contents of the
		// texture buffer at run time, much like you update a constant buffer,
		// set 'allowDynamicUpdate' to 'true'; otherwise, the buffer is immutable.
		TextureBuffer(DFType format, int numElements, bool allowDynamicUpdate);

		DFType GetFormat() const;

		// Access to texture buffer members using the layout of a shader
		// program itself is allowed as long as you have attached the constant
		// buffer to a shader first.
		//   std::shared_ptr<VertexShader> vshader = <some shader>;
		//   std::shared_ptr<TextureBuffer> tbuffer = <buffer for the shader>;
		//   vshader->Set("MyTBuffer", tbuffer);
		// Now you can use SetMember/GetMember calls successfully.  In these
		// calls, you are required to specify the correct type T for the member.
		// No checking is performed for the size of the input; i.e., too large a
		// 'value' will cause a memory overwrite within the buffer.  The code
		// does test to ensure that no overwrite occurs outside the buffer.

		std::vector<MemberLayout>& GetLayout();

		// Test for existence of a member with the specified name.
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
		DFType mFormat;

		// Allow the Shader class to set the data to avoid having to expose
		// setting the lookup table via a public interface.  The 'const' is
		// to allow Shader to call SetLayout from a 'const' TextureBuffer.
		friend class Shader;
		mutable std::vector<MemberLayout> mLayout;

	public:
		// For use by the Shader class for storing reflection information.
		static int const shaderDataLookup = 1;
	};
}
