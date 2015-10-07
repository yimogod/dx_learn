#include "Color.h"

Color::Color(){
    r = 255;
    g = 255;
    b = 255;

    rf = 1.0f;
    gf = 1.0f;
    bf = 1.0f;
    a = 1.0f;
}

Color::Color(int red, int green, int blue, float alpha){
    setColor(red, green, blue, alpha);
}

Color::Color(float red, float green, float blue, float alpha){
    setColor(red, green, blue, alpha);
}

void Color::setColor(int red, int green, int blue, float alpha){
    r = red;
    g = green;
    b = blue;
    
    rf = r / 256.0f;
    gf = g / 256.0f;
    bf = b / 256.0f;
    
    a = alpha;
}

void Color::setColor(float red, float green, float blue, float alpha){
    rf = red;
    gf = green;
    bf = green;
    
    r = (int)(rf * 256.0f);
    g = (int)(gf * 256.0f);
    b = (int)(bf * 256.0f);
    
    a = alpha;
}
