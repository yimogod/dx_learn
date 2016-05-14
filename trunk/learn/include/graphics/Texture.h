#pragma once

#include <graphics/DataFormat.h>
#include <graphics/GraphicsResource.h>
#include <array>
#include <functional>
#include <vector>

namespace plu{
	class Texture : public GraphicsResource{
	protected:
		//dim������ߵ�ά��
		//itemNum������м���ͼ
		Texture(int itemNum, DFType format, int numDimensions, int dim0, int dim1, int dim2, bool hasMipmaps);

	public:
		// texture���м���ͼ
		int GetNumItems();
		DFType GetFormat();
		int GetNumDimensions();
		int GetDimension(int i);

		// Subresource information.
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

		// Subresource indexing:  index = numLevels*item + level
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
		// Support for computing the numElements parameter for the Resource
		// constructor.  This is necessary when mipmaps are requested.
		static int GetTotalElements(int numItems,
			int dim0, int dim1, int dim2,
			bool hasMipmaps);

		int mNumItems;
		DFType mFormat;
		int mNumDimensions;
		int mNumLevels;
		std::array<std::array<unsigned int, 3>, MAX_MIPMAP_LEVELS> mLDimension;
		std::array<unsigned int, MAX_MIPMAP_LEVELS> mLNumBytes;
		//���е�item(����ͼƬ)�����е�mipmap���ڴ�ƫ������
		std::vector<std::array<int, MAX_MIPMAP_LEVELS>> mLOffset;
		bool mHasMipmaps;
		bool mAutogenerateMipmaps;
	};

	typedef std::function<void(std::shared_ptr<Texture> const&)> TextureUpdater;
	typedef std::function<void(std::shared_ptr<Texture> const&, unsigned int)> TextureLevelUpdater;
}
