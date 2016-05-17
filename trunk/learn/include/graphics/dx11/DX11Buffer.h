#pragma once

#include <graphics/Buffer.h>
#include <graphics/dx11/DX11Resource.h>

namespace plu{
	class DX11Buffer : public DX11Resource{
	public:
		// Abstract base class.
		virtual ~DX11Buffer();
	protected:
		DX11Buffer(Buffer* buffer);

	public:
		Buffer* GetBuffer();
		ID3D11Buffer* GetDXBuffer();

		// Copy data from CPU to GPU via mapped memory.  Buffers use only
		// subresource 0, so the subresource index (sri) is not exposed.
		virtual bool Update(ID3D11DeviceContext* context);
		virtual bool CopyCpuToGpu(ID3D11DeviceContext* context);
		virtual bool CopyGpuToCpu(ID3D11DeviceContext* context);

	private:
		// Buffers use only subresource 0, so these overrides are stubbed out.
		virtual bool Update(ID3D11DeviceContext* context, int sri);
		virtual bool CopyCpuToGpu(ID3D11DeviceContext* context, int sri);
		virtual bool CopyGpuToCpu(ID3D11DeviceContext* context, int sri);

	protected:
		// Support for creating staging buffers.
		void CreateStaging(ID3D11Device* device, D3D11_BUFFER_DESC& bf);
	};
}
