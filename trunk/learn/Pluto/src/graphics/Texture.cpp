#include <graphics/Texture.h>
#include <math/BitHacks.h>

using namespace plu;

Texture::Texture(int itemNum, DFType format, int numDimensions,
		int dim0, int dim1, int dim2, bool hasMipmaps)
    :GraphicsResource(GetTotalElements(itemNum, dim0, dim1, dim2, hasMipmaps),
        DataFormat::GetNumBytesPerStruct(format)){
	mNumItems = itemNum;
	mFormat = format;
	mNumDimensions = numDimensions;
	mNumLevels = 1;
	mHasMipmaps = hasMipmaps;
	mAutogenerateMipmaps = false;
    _graphType = GT_TEXTURE;

    if (mNumDimensions < 1 || mNumDimensions > 3){
        mNumDimensions = 1;
    }

    // ��0�������mipmap��ά����Ϣ
    for (int level = 0; level < MAX_MIPMAP_LEVELS; ++level){
        mLDimension[level][0] = 0;
        mLDimension[level][1] = 0;
        mLDimension[level][2] = 0;
        mLNumBytes[level] = 0;
    }

	//�м���ͼƬ, �Ϳ��ټ���vectorԪ��, Ȼ����0������е�������Ϊ�ڴ�ƫ��
	//�������������ڴ�ƫ��
	mLOffset.reserve(itemNum);
    for (int item = 0; item < mNumItems; ++item){
        for (int level = 0; level < MAX_MIPMAP_LEVELS; ++level){
            mLOffset[item][level] = 0;
        }
    }

    // Ĭ�ϵ�0ά�Ⱦ���ͼƬԭʼ�ߴ�
    mLDimension[0][0] = dim0;
    mLDimension[0][1] = dim1;
    mLDimension[0][2] = dim2;
	//item size��������ÿ������ռ�õ��ڴ�,����mLNumBytes[0]ԭʼͼռ�õ��ڴ�
    mLNumBytes[0] = dim0 * dim1 * dim2 * _itemSize;

	//��mipmap, �����е��ڴ�ƫ��λ�ö���Ҫ����
	//��������˹���֤�ֱ�����2�Ĵ���
    if (mHasMipmaps)
	{
		//������2�ļ�����, ���м���mipmap
        int log0 = Log2OfPowerOfTwo(dim0);
        int log1 = Log2OfPowerOfTwo(dim1);
        int log2 = Log2OfPowerOfTwo(dim2);
        mNumLevels = 1 + std::max(log0, std::max(log1, log2));
        for (int level = 1; level < mNumLevels; ++level){
			//dim0 / 2, ���Ǽ�����һ��ĳߴ�
            if (dim0 > 1) { dim0 >>= 1; }
            if (dim1 > 1) { dim1 >>= 1; }
            if (dim2 > 1) { dim2 >>= 1; }

            mLNumBytes[level] = dim0 * dim1 * dim2 * _itemSize;
            mLDimension[level][0] = dim0;
            mLDimension[level][1] = dim1;
            mLDimension[level][2] = dim2;
        }

        int numBytes = 0;
        for (int item = 0; item < mNumItems; ++item){
            for (int L0 = 0, L1 = 1; L1 < mNumLevels; ++L0, ++L1){
                numBytes += mLNumBytes[L0];
                mLOffset[item][L1] = numBytes;
            }
        }
    }
	else//û��mipmap, ������ͼƬ���ڴ�ƫ�ƵĶ���ͼƬ�����index * ����ͼƬ���ڴ�ռ��
	{
        for (int item = 1; item < mNumItems; ++item){
            mLOffset[item][0] = item * mLNumBytes[0];
        }
    }
}

int Texture::GetIndex(int item, int level){
    if (item < mNumItems && level < mNumLevels)
        return mNumLevels * item + level;
    
    return 0;
}

Texture::Subresource Texture::GetSubresource(int index){
    Subresource sr;
    if (index < GetNumSubresources()){
        sr.item = index / mNumLevels;
        sr.level = index % mNumLevels;
        sr.data = const_cast<char*>(GetDataFor(sr.item, sr.level));
        sr.rowPitch = mLDimension[sr.level][0] * _itemSize;
        sr.slicePitch = mLDimension[sr.level][1] * sr.rowPitch;
    }else{
        //LogError("Invalid input.");
        sr.item = 0;
        sr.level = 0;
        sr.data = nullptr;
        sr.rowPitch = 0;
        sr.slicePitch = 0;
    }
    return sr;
}

void Texture::AutogenerateMipmaps(){
    if (mHasMipmaps){
		mAutogenerateMipmaps = true;
        //mUsage = SHADER_OUTPUT;
    }
}

int Texture::GetTotalElements(int numItems, int dim0, int dim1, int dim2, bool hasMipmaps){
    int numElementsPerItem = dim0 * dim1 * dim2;
    if (hasMipmaps){
        int log0 = Log2OfPowerOfTwo(dim0);
        int log1 = Log2OfPowerOfTwo(dim1);
        int log2 = Log2OfPowerOfTwo(dim2);
        int numLevels = 1 + std::max(log0, std::max(log1, log2));
        for (int level = 1; level < numLevels; ++level){
            if (dim0 > 1) { dim0 >>= 1; }
            if (dim1 > 1) { dim1 >>= 1; }
            if (dim2 > 1) { dim2 >>= 1; }

            numElementsPerItem += dim0 * dim1 * dim2;
        }
    }
    return numItems * numElementsPerItem;
}


int Texture::GetNumItems(){
	return mNumItems;
}

DFType Texture::GetFormat(){
	return mFormat;
}

int Texture::GetNumDimensions(){
	return mNumDimensions;
}

int Texture::GetDimension(int i){
	return mLDimension[0][i];
}

bool Texture::HasMipmaps(){
	return mHasMipmaps;
}

int Texture::GetNumLevels(){
	return mNumLevels;
}

int Texture::GetDimensionFor(int level, int i){
	return mLDimension[level][i];
}

int Texture::GetNumElementsFor(int level){
	return mLNumBytes[level] / _itemSize;
}

int Texture::GetNumBytesFor(int level){
	return mLNumBytes[level];
}

int Texture::GetOffsetFor(int item, int level){
	return mLOffset[item][level];
}

char* Texture::GetDataFor(int item, int level){
	return _rawData + mLOffset[item][level];
}

template <typename T>
const* Texture::GetFor(int item, int level){
	return reinterpret_cast<T const*>(GetDataFor(item, level));
}

int Texture::GetNumSubresources(){
	return mNumItems * mNumLevels;
}

bool Texture::WantAutogenerateMipmaps(){
	return mAutogenerateMipmaps;
}