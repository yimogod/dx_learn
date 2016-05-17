#pragma once
#include <d3d11.h>
#include <string>
#include <graphics/GraphicsResource.h>
#include <graphics/dx11/DX11GraphicsObject.h>

namespace plu{
	class DX11Resource : public DX11GraphicsObject{
	public:
		virtual ~DX11Resource();
	protected:
		DX11Resource(GraphicsResource* gtResource);

	public:
		// Member access.
		GraphicsResource* GetResource();
		ID3D11Resource* GetDXResource();
		ID3D11Resource* GetStagingResource();

		// Copy data from CPU to GPU via mapped memory for the specified
		// subresource.  The mapping is successful iff the returned struct's
		// pData member is not null.  The caller is responsible for using the
		// row pitch and slice pitch correctly when writing to the memory.
		//也可以说是lock, 就是锁住gpu中这块内存, 然后cpu去操作, 然后gpu再渲染
		D3D11_MAPPED_SUBRESOURCE MapForWrite(ID3D11DeviceContext* context, int sri);
		void Unmap(ID3D11DeviceContext* context, int sri);

		// Copy from CPU (mGTObject memory) to GPU (mDXObject memory).  The first
		// function copies the specified subresource.  The second function copies
		// all subresources.
		virtual bool Update(ID3D11DeviceContext* context, int sri) = 0;
		virtual bool Update(ID3D11DeviceContext* context) = 0;

		// Copy from CPU to GPU using staging buffers.  The first function copies
		// the specified subresource.  The second function copies all subresources.
		virtual bool CopyCpuToGpu(ID3D11DeviceContext* context, int sri) = 0;
		virtual bool CopyCpuToGpu(ID3D11DeviceContext* context) = 0;

		// Copy from GPU to CPU using staging buffers.  The first function copies
		// the specified subresource.  The second function copies all subresources.
		virtual bool CopyGpuToCpu(ID3D11DeviceContext* context, int sri) = 0;
		virtual bool CopyGpuToCpu(ID3D11DeviceContext* context) = 0;

	protected:
		// Support for copying between CPU and GPU.
		//判断此resource是否各个数据都被填充, 可以用来在cpu和gpu间拷贝操作
		bool PreparedForCopy(D3D11_CPU_ACCESS_FLAG access);

		// A staging buffer is used for copying between CPU and GPU memory.
		ID3D11Resource* mStaging;

		// Mapping from Resource::CopyType to D3D11_CPU_ACCESS_FLAG.
		static int const msStagingAccess[];

	public:
		virtual void SetName(std::string const& name);
	};

}
