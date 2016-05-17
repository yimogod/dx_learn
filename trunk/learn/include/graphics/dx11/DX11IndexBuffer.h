#pragma once
#include <graphics/IndexBuffer.h>
#include <graphics/dx11/DX11Buffer.h>

namespace plu{
	class DX11IndexBuffer : public DX11Buffer{
	public:
		// Construction and destruction.
		virtual ~DX11IndexBuffer();
		DX11IndexBuffer(ID3D11Device* device, IndexBuffer* vbuffer);
		static DX11IndexBuffer* Create(ID3D11Device* device, GraphicsObject* object);

		// Member access.
		IndexBuffer* GetIndexBuffer() const;

		// Drawing support.
		void Enable(ID3D11DeviceContext* context);
		void Disable(ID3D11DeviceContext* context);

	private:
		DXGI_FORMAT mFormat;
	};
}
