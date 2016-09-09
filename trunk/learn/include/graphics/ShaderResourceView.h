#pragma once
#include <d3d11_1.h>

class ShaderResourceView{
public:
	ShaderResourceView();
	~ShaderResourceView();

	bool CreateTexture(ID3D11Device* device);
	void AddTexturePath(const wchar_t* path);
	inline void BindShaderResource(ID3D11DeviceContext* context, UINT StartSlot);
private:
	/*����ͼ��Դ���õ�resource view, ���һ������Ҫ����ͼ, ��ô���ǵ�_resView�ͻ��Ǹ�����*/
	int _resViewNum = 0;
	ID3D11ShaderResourceView* _resView[8];

	//�洢·����Ϣ, ��������ID3D11ShaderResourceView
	const wchar_t* _texturePathList[8];
};

inline void ShaderResourceView::BindShaderResource(ID3D11DeviceContext* context, UINT StartSlot){
	context->PSSetShaderResources(StartSlot, _resViewNum, _resView);
}