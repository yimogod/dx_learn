#pragma once
#include <string>
#include <d3d11_1.h>

class ShaderResourceView{
public:
	ShaderResourceView();
	~ShaderResourceView();

	inline int GetResViewNum() const;

	bool CreateTexture(ID3D11Device* device);
	void AddTexturePath(const std::wstring &path);
	inline void BindShaderResource(ID3D11DeviceContext* context, UINT StartSlot);
private:
	/*����ͼ��Դ���õ�resource view, ���һ������Ҫ����ͼ, ��ô���ǵ�_resView�ͻ��Ǹ�����*/
	int _resViewNum = 0;

	//�洢·����Ϣ, ��������ID3D11ShaderResourceView
	std::wstring _texturePathList[8];
	ID3D11ShaderResourceView* _resView[8];
};

inline void ShaderResourceView::BindShaderResource(ID3D11DeviceContext* context, UINT StartSlot){
	context->PSSetShaderResources(StartSlot, _resViewNum, _resView);
}

inline int ShaderResourceView::GetResViewNum() const{
	return _resViewNum;
}