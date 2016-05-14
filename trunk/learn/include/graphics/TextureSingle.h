#pragma once
#include <graphics/Texture.h>

namespace plu{
	class TextureSingle : public Texture{
	protected:
		// Abstract base class.
		TextureSingle(DFType format, int numDimensions,
			int dim0, int dim1, int dim2, bool hasMipmaps);

	public:
		// Mipmap information.
		int GetOffsetFor(int level);
		char* GetDataFor(int level);

		template <typename T>
		T* GetFor(int level);
	public:
		// For use by the Shader class for storing reflection information.
		static int const shaderDataLookup = 4;
	};
}
