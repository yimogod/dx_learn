#include <graphics/dx11/DX11GraphicsObject.h>
using namespace plu;

DX11GraphicsObject::DX11GraphicsObject(GraphicsObject* graphObj){
	_pluObj = graphObj;
	_dxObj = nullptr;
}


DX11GraphicsObject::~DX11GraphicsObject(){
}
