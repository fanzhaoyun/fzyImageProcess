#pragma once

#ifndef IMAGE_STRUCT_H
#define IMAGE_STRUCT_H


#define GRAY 0
#define COLORIMG 1

class ImStruct
{
public:
	int deep; //深度
	int chanel; //通道数
	int type; //图像类型
	BYTE* data; //数据
	int width; //宽
	int height; //高
public:
	ImStruct();
	ImStruct(int dp, int chl, int te, unsigned char* da, int w, int h);
	ImStruct(const ImStruct& img);
	void create(int width, int height,int te);
};

#endif // !IMAGE_STRUCT_H