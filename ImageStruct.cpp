#include<Windows.h>

#include"ImageStruct.h"

ImStruct::ImStruct() {}

ImStruct::~ImStruct() {
	delete data;
}

ImStruct::ImStruct(int dp, int chl, int te, unsigned char* da, int w, int h) :deep(dp), chanel(chl), type(te), data(da), width(w), height(h) {}

ImStruct::ImStruct(const ImStruct& img) {
	deep = img.deep;
	chanel = img.chanel;
	type = img.type;
	data = new BYTE[img.width * img.height];
	memcpy(data, img.data, sizeof(BYTE)*img.width * img.height);
	width = img.width;
	height = img.height;
}

void ImStruct::create(int widtht, int heightt, int te) {
	if (te == GRAY) {
		deep = 8;
		chanel = 1;
		type = te;
		data = new BYTE[widtht * heightt];
		//memset(data, 0, sizeof(BYTE)*widtht * heightt);
		width = widtht;
		height = heightt;
	}
	else {
		deep = 24;
		chanel = 3;
		type = te;
		data = new BYTE[widtht * 3 * heightt];
		width = widtht;
		height = heightt;
	}
	
}