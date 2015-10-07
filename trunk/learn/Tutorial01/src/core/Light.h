#pragma once

#include <iostream>
#include "Color.h"
#include "Vertex.h"
#include "Vector3D.h"

class Light{
public:
    static const int TYPE_AMBIENT = 1;
    static const int TYPE_INFINITE = 2;
    static const int TYPE_POINT = 4;
    
    Light();
    
    /* 开关 */
    bool turnON;
    /* 环境光, 平行光, 点光源, 聚光灯 */
    int type;
    
    /* 光照颜色 */
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    
    /* 位置朝向 */
    Vector3D pos;
    Vector3D dir;
    
    /* 衰减系数 */
    float kc, kl, kq;
};
