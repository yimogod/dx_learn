#pragma once

#include <string>

namespace plu{
	//����shader ���������layout, ��������D3D11_INPUT_ELEMENT_DESC,����VS INPUT
	//Ҳ�������ڴ���Constant Buffer
	struct MemberLayout{
		std::string name;
		int offset;
		int numElements;
	};
}
