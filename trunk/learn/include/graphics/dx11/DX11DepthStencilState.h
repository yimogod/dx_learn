#pragma once
#include <d3d11.h>
#include <graphics/GraphicsObject.h>
#include <graphics/DepthStencilState.h>
#include <graphics/dx11/DX11DrawingState.h>

namespace plu{
	class DX11DepthStencilState : public DX11DrawingState{
	public:
		virtual ~DX11DepthStencilState();
		DX11DepthStencilState(ID3D11Device* device, DepthStencilState* state);
		/*静态函数, 创建深度模板状态*/
		static DX11DepthStencilState* create(ID3D11Device* device, GraphicsObject* object);

		DepthStencilState* getDepthStencilState();
		ID3D11DepthStencilState* getDXDepthStencilState();

		// Enable the depth-stencil state.
		void enable(ID3D11DeviceContext* context);
	private:
		// Conversions from GTEngine values to DX11 values.
		static D3D11_DEPTH_WRITE_MASK const msWriteMask[];
		static D3D11_COMPARISON_FUNC const msComparison[];
		static D3D11_STENCIL_OP const msOperation[];
	};
}