#pragma once
#include <d3d11.h>
#include <graphics/GraphicsObject.h>

namespace plu{
	class DX11GraphicsObject{
	public:
		virtual ~DX11GraphicsObject();

	protected:
		DX11GraphicsObject(GraphicsObject* graphObj);

		GraphicsObject* _pluObj;
		/*缓存引擎图形类(blendstate, raststate==)对应的dx11的对象*/
		ID3D11DeviceChild* _dxObj;
	};
}