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

		// Internal staging buffer generation.  These control creation of staging
		// buffers to support copies between CPU, staging buffers, and GPU.  You
		// must set the copy type before binding the resource to an engine.
		enum CopyType{
			COPY_NONE,           // 0 (default)
			COPY_CPU_TO_STAGING, // D3D11_CPU_ACCESS_WRITE
			COPY_STAGING_TO_CPU, // D3D11_CPU_ACCESS_READ
			COPY_BIDIRECTIONAL   // D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ
		};

	public:
		~GraphicsResource();

		int GetItemNum();
		int GetItemSize();
		int GetTotalBytes();

		void SetUsage(Usage usage);
		Usage GetUsage();

		void SetCopyType(CopyType copyType);
		CopyType GetCopyType();

		//设置资源包含的数据
		void ResetData();
		void SetData(char* data);
		char* GetData();

	protected:
		GraphicsResource(int itemNum, int itemSize);

	protected:
		int _itemNum;
		int _itemSize;
		int _totalBytes;

		int _offset;

		Usage _usage;
		CopyType mCopyType;

		char* _rawData;
	};

}