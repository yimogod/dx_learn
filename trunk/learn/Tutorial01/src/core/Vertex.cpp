#include "Vertex.h"

Vertex::Vertex(){
    x = y = z = 0;
    w = 1;
}

Vertex::Vertex(float px, float py, float pz){
    x = px;
    y = py;
    z = pz;
    w = 1;
}

Vertex::Vertex(float px, float py, float pz, float pw){
    x = px;
    y = py;
    z = pz;
    w = pw;
}

void Vertex::setColor(int red, int green, int blue){
	if (red > 255)red = 255;
	if (green > 255)green = 255;
	if (blue > 255)blue = 255;
	color.setColor(red, green, blue);
}

void Vertex::copyFrom(Vertex *source){
    x = source->x;
    y = source->y;
    z = source->z;
}

void Vertex::copyNormal(Vector3D *source){
    normal.x = source->x;
    normal.y = source->y;
    normal.z = source->z;
}

void Vertex::toString(){
    std::cout << x << ", " << y << ", " << z << std::endl;
}