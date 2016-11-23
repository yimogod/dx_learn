#pragma once
#include <string>
#include <istream>

void print_pwd();

bool float_equal(float f1, float f2);

bool float_less(float f1, float f2, float v);

void swap_float(float* f1, float* f2);

void swap_int(int* f1, int* f2);

int str_2_int(std::string& str);

int char_2_int(char c);

void str_2_wstr(const std::string& src, std::wstring& dest);