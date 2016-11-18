#pragma once
#include <graphics/DXShader.h>
class PixelShader : public Shader{
public:
	PixelShader();
	PixelShader(wchar_t* fileName);
	~PixelShader();

	virtual void Reset();
	bool CreatePixelShader(ID3D11Device* device);
	inline void PSSetShader(ID3D11DeviceContext* context);
private:
	ID3D11PixelShader* _ps = nullptr;
};

inline void PixelShader::PSSetShader(ID3D11DeviceContext* context){
	context->PSSetShader(_ps, nullptr, 0);
}
