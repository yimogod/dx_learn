#pragma once

#include <graphics/DataFormat.h>
#include <graphics/GraphicsResource.h>
#include <array>
#include <functional>
#include <vector>

namespace plu{
	class Texture : public GraphicsResource{
	protected:
		//dim代表长宽高的维度
		//itemNum代表会有几张图
		Texture(int itemNum, DFType format, int numDimensions, int dim0, int dim1, int dim2, bool hasMipmaps);

	public:
		// texture中有几张图
		int GetNumItems();
		DFType GetFormat();
		int GetNumDimensions();
		int GetDimension(int i);

		//子资源, 指的是每个单的贴图数据, 跟图片数和level数相关
		struct Subresource{
			int item;
			int level;
			char* data;
			int rowPitch;
			int slicePitch;
		};

		// Mipmap information.
		enum { MAX_MIPMAP_LEVELS = 16 };
		bool HasMipmaps();
		int GetNumLevels();
		int GetDimensionFor(int level, int i) ;
		int GetNumElementsFor(int level);
		int GetNumBytesFor(int level);
		int GetOffsetFor(int item, int level);
		char* GetDataFor(int item, int level);

		template <typename T>
		T* GetFor(int item, int level);

		// 子资源的个数  index = numLevels*item + level
		int GetNumSubresources();
		int GetIndex(int item, int level);
		Subresource GetSubresource(int index);

		// Request that the GPU compute mipmap levels when the base-level texture
		// data is modified.  The AutogenerateMipmaps call should be made before
		// binding the texture to the engine.  If the texture does not have mipmaps,
		// the AutogenerateMipmaps call will not set mAutogenerateMipmaps to true.
		void AutogenerateMipmaps();
		bool WantAutogenerateMipmaps();

	protected:
		//计算有多少像素总共
		static int GetTotalElements(int numItems,
			int dim0, int dim1, int dim2,
			bool hasMipmaps);

		//本texture用到了几张图片
		int mNumItems;
		//texture导入格式, 用于传给显卡. 跟DX的格式一一对应
		DFType mFormat;
		//texture维度, 比如单张图片就是2
		int mNumDimensions;
		//mip level个数, 是三维中最大的level数
		int mNumLevels;
		//第一个index是miplevel, 第二个索引是维度, 存储的是所有mip level下的维度信息
		std::array<std::array<int, 3>, MAX_MIPMAP_LEVELS> mLDimension;
		//存储每个mip level所占有的内存
		std::array<int, MAX_MIPMAP_LEVELS> mLNumBytes;
		//所有的item(单张图片)的所有的mip level的内存偏移数据
		std::vector<std::array<int, MAX_MIPMAP_LEVELS>> mLOffset;
		bool mHasMipmaps;
		bool mAutogenerateMipmaps;
	};

	typedef std::function<void(std::shared_ptr<Texture> const&)> TextureUpdater;
	typedef std::function<void(std::shared_ptr<Texture> const&, unsigned int)> TextureLevelUpdater;
}
