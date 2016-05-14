#pragma once
#include <graphics/TextureSingle.h>

namespace plu{
	//һ�ų����ε���ͼ
	class Texture2 : public TextureSingle{
	public:
		Texture2(DFType format, int width, int height, bool hasMipmaps = false);

		// Texture dimensions.
		int GetWidth();
		int GetHeight();

		// If you intend to share this texture among DX11Engine objects, call this
		// function before binding the texture to the engine.  The Texture2 usage
		// must be Resource::GPU_RW.
		void MakeShared();
		bool IsShared();

	protected:
		bool mShared;
	};
}
