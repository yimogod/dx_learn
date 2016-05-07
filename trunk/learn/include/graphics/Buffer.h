#pragma once
#include <graphics/GraphicsResource.h>

namespace plu{
	class Buffer : public GraphicsResource{
	public:
		~Buffer();
	protected:
		Buffer(int itemNum, int itemSize);
	};
}