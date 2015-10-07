#pragma once

#include <iostream>

/* i need use int to instead of this color class */
class Color{
public:
    Color();
    Color(int red, int green, int blue, float alpha = 1.0f);
    Color(float red, float green, float blue, float alpha = 1.0f);

    /* 0 ~ 255 */
    int r, g, b;
    /* 0.0f ~ 1.0f */
    float rf, gf, bf;
    /* 0.0f ~ 1.0f */
    float a;
    
    void setColor(int red, int green, int blue, float alpha = 1.0f);
    void setColor(float red, float green, float blue, float alpha = 1.0f);
};
