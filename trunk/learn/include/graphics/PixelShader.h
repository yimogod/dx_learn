#pragma once
#include <graphics/Shader.h>
class PixelShader : public Shader{
public:
	PixelShader();
	PixelShader(wchar_t* fileName);
	~PixelShader();

	bool CreatePixelShader(ID3D11Device* device);
	inline void PSSetShader(ID3D11DeviceContext* context);
private:
	ID3D11PixelShader* _ps;
};

inline void PixelShader::PSSetShader(ID3D11DeviceContext* context){
	context->PSSetShader(_ps, nullptr, 0);
}
