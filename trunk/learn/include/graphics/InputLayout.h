#pragma once
#include <graphics/DXInclude.h>

class InputLayout
{
public:
	InputLayout();
	~InputLayout();

protected:
	ID3D11InputLayout*  _vertexLayout = nullptr;
};
