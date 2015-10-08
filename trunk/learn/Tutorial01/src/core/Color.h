#pragma once
struct Color{
	/* 0.0f ~ 1.0f */
	float r;
	float g;
	float b;
    /* 0.0f ~ 1.0f */
    float a;

	Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f){}
	Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.0f) {}

	Color& operator= (const Color& color) { r = color.r; g = color.g; b = color.b; a = color.a; return *this; }
};
