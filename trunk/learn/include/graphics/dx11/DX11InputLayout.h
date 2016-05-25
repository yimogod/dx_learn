#pragma once
#include <d3d11.h>

#include <graphics/Shader.h>
#include <graphics/VertexBuffer.h>

namespace plu{
	class DX11InputLayout{
	public:
		// Construction and destruction.
		~DX11InputLayout();
		DX11InputLayout(ID3D11Device* device, VertexBuffer* vbuffer, Shader* vshader);

		// Support for drawing geometric primitives.
		void Enable(ID3D11DeviceContext* context);
		void Disable(ID3D11DeviceContext* context);

		// Support for the DX11 debug layer; see comments in the file
		// GteDX11GraphicsObject.h about usage.
		HRESULT SetName(std::string& name);
		std::string& GetName();

	private:
		ID3D11InputLayout* mLayout;
		int mNumElements;
		D3D11_INPUT_ELEMENT_DESC mElements[VA_MAX_ATTRIBUTES];
		std::string mName;

		// Conversions from GTEngine values to DX11 values.
		static char const* msSemantic[VA_NUM_SEMANTICS];
	};

}
