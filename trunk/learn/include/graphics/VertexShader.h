#pragma once
#include <graphics/Shader.h>
class VertexShader : public Shader{
public:
	VertexShader();
	VertexShader(wchar_t* fileName);

	~VertexShader();


	bool CreateVertexShader(ID3D11Device* device, InputLayout &inputLayout);
	inline void VSSetShader(ID3D11DeviceContext* context);
private:
	ID3D11VertexShader* _vs;
};

inline void VertexShader::VSSetShader(ID3D11DeviceContext* context){
	context->VSSetShader(_vs, nullptr, 0);
}
