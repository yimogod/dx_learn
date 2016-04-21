#include <sstream>
#include <util/CUtil.h>
#include <Windows.h>
#include <stringapiset.h>

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

int str_2_int(string& str){
	int v;
	if(!(istringstream(str) >> v)) v = 0;
	return v;
}

int char_2_int(char c){
	return c - 48;
}

void str_2_wstr(string& src, wstring& dest){
	string temp = src;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t * wszUtf8 = new wchar_t[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	dest = wszUtf8;
	delete[] wszUtf8;
}