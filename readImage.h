#pragma once
#ifndef READ_IMAGE_H
#define READ_IMAGE_H


#include<Windows.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>

#include"ImageStruct.h"

using namespace std;

ImStruct* ReadImage(ImStruct* imStruct, const char* filename);
bool WriteImage(ImStruct* img, const char* filename);
void ColortoGray(const ImStruct * src, ImStruct * dst);
void GetImInform(const char* filename);
void GetImInform(const ImStruct* src);
ImStruct RgbtoHsv(const ImStruct* src);
ImStruct RgbtoYuv(const ImStruct* src);
ImStruct RgbtoHsi(const ImStruct* src);
void Colorto2(const ImStruct* src, const ImStruct *dst);
void Grayto2(const ImStruct* src, const ImStruct *dst);

#endif // !READ_IMAGE_H