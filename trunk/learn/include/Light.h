#pragma once

#include <iostream>
#include "Color.h"
#include "Vector3D.h"

class Light{
public:
    static const int TYPE_DIRECTION = 1;
    static const int TYPE_POINT = 2;
    
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
