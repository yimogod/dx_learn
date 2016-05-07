#pragma once
#include <graphics/GraphicsObject.h>

namespace plu{
	class GraphicsResource : public GraphicsObject{
	public:
		enum Usage{
			IMMUTABLE,       // D3D11_USAGE_IMMUTABLE (default)
			DYNAMIC_UPDATE,  // D3D11_USAGE_DYNAMIC
			SHADER_OUTPUT    // D3D11_USAGE_DEFAULT
		};

	public:
		~GraphicsResource();

		int getItemNum();
		int getItemSize();
		int getTotalBytes();

		void setUsage(Usage usage);
		Usage getUsage();

	protected:
		GraphicsResource(int itemNum, int itemSize);

	protected:
		int _itemNum;
		int _itemSize;
		int _totalBytes;

		int _offset;

		Usage _usage;
		char* _rawData;
	};

}