#pragma once

#include <graphics/ConstantBuffer.h>
#include <graphics/dx11/DX11Buffer.h>

namespace plu{
	class DX11ConstantBuffer : public DX11Buffer{
	public:
		virtual ~DX11ConstantBuffer();
		DX11ConstantBuffer(ID3D11Device* device, ConstantBuffer* cbuffer);
		ConstantBuffer* GetConstantBuffer();

	public:
		static DX11ConstantBuffer* Create(ID3D11Device* device, GraphicsObject* object);
	};

}
