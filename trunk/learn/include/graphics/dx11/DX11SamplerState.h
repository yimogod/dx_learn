#pragma once

#include <graphics/SamplerState.h>
#include <graphics/dx11/DX11DrawingState.h>

namespace plu{
	class DX11SamplerState : public DX11DrawingState{
	public:
		// Construction and destruction.
		virtual ~DX11SamplerState();
		DX11SamplerState(ID3D11Device* device,	SamplerState* samplerState);
		static DX11SamplerState* create(ID3D11Device* device,	GraphicsObject* object);

		// Member access.
		SamplerState* getSamplerState();
		ID3D11SamplerState* getDXSamplerState();

	private:
		// Conversions from GTEngine values to DX11 values.
		static D3D11_FILTER const msFilter[];
		static D3D11_TEXTURE_ADDRESS_MODE const msMode[];
		static D3D11_COMPARISON_FUNC const msComparison[];
	};
}
