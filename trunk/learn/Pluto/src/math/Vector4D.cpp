#include <math/algebra/Vector4D.h>
using namespace plu;

Vector4D::Vector4D(){
	x = y = z = w = 1.0f;
}

Vector4D::Vector4D(float px, float py, float pz, float pw){
	x = px;
	y = py;
	z = pz;
	w = pw;
}

Vector4D::~Vector4D(){
}
