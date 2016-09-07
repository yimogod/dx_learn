#pragma once
#include <d3d11_1.h>

class InputLayout
{
public:
	InputLayout();
	~InputLayout();

protected:
	ID3D11InputLayout*  _vertexLayout = nullptr;
};
