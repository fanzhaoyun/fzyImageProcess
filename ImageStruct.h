#pragma once

#ifndef IMAGE_STRUCT_H
#define IMAGE_STRUCT_H


#define GRAY 0
#define COLORIMG 1

class ImStruct
{
public:
	int deep; //���
	int chanel; //ͨ����
	int type; //ͼ������
	BYTE* data; //����
	int width; //��
	int height; //��
public:
	ImStruct();
	ImStruct(int dp, int chl, int te, unsigned char* da, int w, int h);
	ImStruct(const ImStruct& img);
	void create(int width, int height,int te);
};

#endif // !IMAGE_STRUCT_H