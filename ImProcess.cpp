#include"ImProcess.h"

/*
���룺
�����
*/
void line(const ImStruct *src, ImStruct *dst, double k, double b) {
	int width, height;
	width = dst->width;
	height = dst->height;
	BYTE LUT[256];
	for (int i = 0; i < 256; i++) {
		int temp = i * k + b;
		LUT[i] = (temp < 0) ? 0 : (temp > 255) ? 255 : temp;  //tempС��0ȡ0������255ȡ255
	}
	if (dst->type == COLORIMG) {
		width *= 3;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dst->data[i * width + j] = LUT[src->data[i * width + j]];
		}
	}
}

