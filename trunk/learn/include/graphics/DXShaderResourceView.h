#pragma once
#include <string>
#include <d3d11_1.h>

//对texture引用的中间层, 会设置shader中用到的texture
//比如vs input的texturecord0
class ShaderResourceView{
public:
	ShaderResourceView();
	~ShaderResourceView();

	inline int GetResViewNum() const;

	void Reset();
	bool CreateTexture(ID3D11Device* device);
	void AddTexturePath(const std::string &path);
	inline void BindShaderResource(ID3D11DeviceContext* context, UINT StartSlot);
private:
	/*对贴图资源引用的resource view, 如果一个面需要多张图, 那么我们的_resView就会是个数组*/
	int _resViewNum = 0;

	//存储路径信息, 用来创建ID3D11ShaderResourceView
	std::wstring _texturePathList[8];
	ID3D11ShaderResourceView* _resView[8];
};

inline void ShaderResourceView::BindShaderResource(ID3D11DeviceContext* context, UINT StartSlot){
	context->PSSetShaderResources(StartSlot, _resViewNum, _resView);
}

inline int ShaderResourceView::GetResViewNum() const{
	return _resViewNum;
}