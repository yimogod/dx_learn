#pragma once

#include <string>

namespace plu{
	//用于shader 传入变量的layout, 可以用于D3D11_INPUT_ELEMENT_DESC,传入VS INPUT
	//也可以用于创建Constant Buffer
	struct MemberLayout{
		std::string name;
		int offset;
		int numElements;
	};
}
