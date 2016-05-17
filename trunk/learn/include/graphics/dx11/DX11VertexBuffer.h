#pragma once

#include <graphics/VertexBuffer.h>
#include <graphics/dx11/DX11Buffer.h>

namespace plu{
	class DX11VertexBuffer : public DX11Buffer{
	public:
		virtual ~DX11VertexBuffer();
		DX11VertexBuffer(ID3D11Device* device, VertexBuffer* vbuffer);

		static DX11VertexBuffer* Create(ID3D11Device* device, GraphicsObject* object);

		VertexBuffer* GetVertexBuffer();

		// Drawing support.
		void Enable(ID3D11DeviceContext* context);
		void Disable(ID3D11DeviceContext* context);
	};

}
