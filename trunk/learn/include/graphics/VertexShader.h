#pragma once
#include <graphics/Shader.h>

class VertexShader : public Shader
{
public:
	VertexShader();
	~VertexShader();

protected:
	ID3D11VertexShader* _vs = nullptr;
};

