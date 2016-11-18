#pragma once
#include <graphics/DXShader.h>
class VertexShader : public Shader{
public:
	VertexShader();
	VertexShader(wchar_t* fileName);

	~VertexShader();

	virtual void Reset();
	bool CreateVertexShader(ID3D11Device* device, InputLayout &inputLayout);
	inline void VSSetShader(ID3D11DeviceContext* context);
private:
	ID3D11VertexShader* _vs = nullptr;
};

inline void VertexShader::VSSetShader(ID3D11DeviceContext* context){
	context->VSSetShader(_vs, nullptr, 0);
}
