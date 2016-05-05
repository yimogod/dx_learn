#pragma once
#include <d3d11.h>
#include <graphics/dx11/DX11DrawingState.h>
#include <graphics/BlendState.h>

namespace plu{
	class DX11BlendState : public DX11DrawingState{
	public:
		virtual ~DX11BlendState();
		DX11BlendState(ID3D11Device* device, BlendState* state);
		static DX11BlendState* create(ID3D11Device* device,	GraphicsObject* object);

		BlendState* getBlendState();
		ID3D11BlendState* getDXBlendState();

		/*开启混合模式*/
		void enable(ID3D11DeviceContext* context);

	private:
		/*自定义的混合状态相关的常量和dx11的混合常量的转换*/
		static D3D11_BLEND const msMode[];
		static D3D11_BLEND_OP const msOperation[];
	};

}