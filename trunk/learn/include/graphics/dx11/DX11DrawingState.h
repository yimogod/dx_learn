#pragma once
#include <graphics/dx11/DX11GraphicsObject.h>
#include <graphics/DrawingState.h>

namespace plu{
	class DX11DrawingState : public DX11GraphicsObject{
	public:
		~DX11DrawingState();
	protected:
		DX11DrawingState(DrawingState* state);
	};
}