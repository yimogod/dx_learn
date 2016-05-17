#pragma once

#include <graphics/Texture.h>
#include <graphics/dx11/DX11Resource.h>

namespace plu{
	class DX11Texture : public DX11Resource{
	public:
		// Abstract base class.
		virtual ~DX11Texture();
	protected:
		// No public construction.  Derived classes use this constructor.
		DX11Texture(Texture* gtTexture);

	public:
		// Member access.
		Texture* GetTexture();
		ID3D11ShaderResourceView* GetSRView();
		ID3D11UnorderedAccessView* GetUAView();

		// Copy of data between CPU and GPU.
		virtual bool Update(ID3D11DeviceContext* context, int sri);
		virtual bool Update(ID3D11DeviceContext* context);
		virtual bool CopyCpuToGpu(ID3D11DeviceContext* context, int sri);
		virtual bool CopyCpuToGpu(ID3D11DeviceContext* context);
		virtual bool CopyGpuToCpu(ID3D11DeviceContext* context, int sri);
		virtual bool CopyGpuToCpu(ID3D11DeviceContext* context);

	protected:
		// Support for copy of row-pitched and slice-pitched (noncontiguous)
		// texture memory.
		static void CopyPitched2(int numRows, int srcRowPitch, void* srcData, int trgRowPitch, void* trgData);

		static void CopyPitched3(int numRows, int numSlices, int srcRowPitch, int srcSlicePitch,
			void* srcData, int trgRowPitch, int trgSlicePitch, void* trgData);

		ID3D11ShaderResourceView* mSRView;
		ID3D11UnorderedAccessView* mUAView;

	public:
		// Support for the DX11 debug layer; see comments in the file
		// GteDX11GraphicsObject.h about usage.
		virtual void SetName(std::string const& name);
	};

}
