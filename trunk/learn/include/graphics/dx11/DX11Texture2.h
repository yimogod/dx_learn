#pragma once
#include <graphics/Texture2.h>
#include <graphics/DX11/DX11TextureSingle.h>

namespace plu{
	class DX11Texture2 : public DX11TextureSingle{
	public:
		// Construction and destruction.
		virtual ~DX11Texture2();
		DX11Texture2(ID3D11Device* device, Texture2* texture);
		DX11Texture2(ID3D11Device* device, DX11Texture2* dxSharedTexture);

		static DX11Texture2* Create(ID3D11Device* device, GraphicsObject* object);
	protected:
		// Constructors for DX11TextureRT and DX11TextureDS.
		DX11Texture2(Texture2* texture);

	public:
		// Member access.
		Texture2* GetTexture();
		ID3D11Texture2D* GetDXTexture();

	protected:
		// Support for construction.
		ID3D11Texture2D* CreateSharedDXObject(ID3D11Device* device);
		void CreateStaging(ID3D11Device* device, D3D11_TEXTURE2D_DESC& tx);
		void CreateSRView(ID3D11Device* device, D3D11_TEXTURE2D_DESC& tx);
		void CreateUAView(ID3D11Device* device, D3D11_TEXTURE2D_DESC& tx);
	};

}
