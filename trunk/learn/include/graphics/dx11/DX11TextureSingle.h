#pragma once

#include <graphics/TextureSingle.h>
#include <graphics/dx11/DX11Texture.h>

namespace plu{
	class DX11TextureSingle : public DX11Texture{
	public:
		// Abstract base class, a shim to distinguish between single textures and
		// texture arrays.
		virtual ~DX11TextureSingle();
	protected:
		// No public construction.  Derived classes use this constructor.
		DX11TextureSingle(TextureSingle* gtTextureSingle);

	public:
		// Member access.
		TextureSingle* GetTextureSingle();
	};

}
