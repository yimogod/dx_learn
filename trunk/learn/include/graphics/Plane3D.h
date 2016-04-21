#ifndef __TEngine__Plane3D__
#define __TEngine__Plane3D__

#include <iostream>

#include <graphics/Vertex.h>
#include <math/algebra/Vector3D.h>

class Plane3D{
public:
    Plane3D();
    
    Vertex point;
    Vector3D normal;
};

#endif /* defined(__TEngine__Plane3D__) */
