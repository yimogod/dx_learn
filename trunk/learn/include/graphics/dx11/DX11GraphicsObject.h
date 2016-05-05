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
		/*��������ͼ����(blendstate, raststate==)��Ӧ��dx11�Ķ���*/
		ID3D11DeviceChild* _dxObj;
	};
}