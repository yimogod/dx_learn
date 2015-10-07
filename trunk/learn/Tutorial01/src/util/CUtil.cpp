#include "CUtil.h"

using namespace::std;

void print_pwd(){
    //char* path = getcwd(NULL, 0);
    //cout << path << endl;
}

bool float_equal(float f1, float f2){
    float df = f1 - f2;
    if (df >= 0 && df <= 0.000001)return true;
    if (df <=0 && df >= -0.000001)return true;
    
    return false;
}

//v is bigger then 0
bool float_less(float f1, float f2, float v) {
	float df = f1 - f2;
	if (df >= 0 && df <= v)return true;
	if (df <= 0 && df >= -v)return true;

	return false;
}


void swap_float(float* f1, float* f2){
    float temp = *f2;
    *f2 = *f1;
    *f1 = temp;
}

void swap_int(int* f1, int* f2){
	int temp = *f2;
	*f2 = *f1;
	*f1 = temp;
}