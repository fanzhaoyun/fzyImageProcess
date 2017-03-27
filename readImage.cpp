/////////////////////////////////////////////////////////////////////////
//
//          2017.3.21 FZY
//
////////////////////////////////////////////////////////////////////////
#include"readImage.h"


/*
//位图文件头定义;
//其中不包含文件类型信息（由于结构体的内存结构决定，
//要是加了的话将不能正确读取文件信息）
typedef struct  tagBITMAPFILEHEADER{
WORD bfType;//文件类型，必须是0x424D，即字符“BM”
DWORD bfSize;//文件大小
WORD bfReserved1;//保留字
WORD bfReserved2;//保留字
DWORD bfOffBits;//从文件头到实际位图数据的偏移字节数
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
DWORD biSize;//信息头大小
LONG biWidth;//图像宽度
LONG biHeight;//图像高度
WORD biPlanes;//位平面数，必须为1
WORD biBitCount;//每像素位数
DWORD  biCompression; //压缩类型
DWORD  biSizeImage; //压缩图像大小字节数
LONG  biXPelsPerMeter; //水平分辨率
LONG  biYPelsPerMeter; //垂直分辨率
DWORD  biClrUsed; //位图实际用到的色彩数
DWORD  biClrImportant; //本位图中重要的色彩数
}BITMAPINFOHEADER; //位图信息头定义

typedef struct tagRGBQUAD{
BYTE rgbBlue; //该颜色的蓝色分量
BYTE rgbGreen; //该颜色的绿色分量
BYTE rgbRed; //该颜色的红色分量
BYTE rgbReserved; //保留值
}RGBQUAD;//调色板定义

*/

ImStruct* ReadImage(ImStruct* imStruct,const char* filename)
{
	BITMAPFILEHEADER imHead; //位图头
	BITMAPINFOHEADER imInfo; //位图头文件信息
	RGBQUAD imPla[256]; //256色调色板
	FILE *fp;
	int width, height;
	if ((fp = fopen(filename, "rb")) == NULL){
		std::cout << "file open fail" << std::endl;
		return NULL;
	}
//	WORD bfType;
//	fread(&bfType, 1, sizeof(WORD), fp); //读取文件类型
	
	//读取文件头和信息头
	fread(&imHead, 1, sizeof(BITMAPFILEHEADER), fp);
	fread(&imInfo, 1, sizeof(BITMAPINFOHEADER), fp);
	if (0x4d42 != imHead.bfType) { //位图文件头标记"BM" 
		std::cout << "This file is not a bmp file" << std::endl;
		fclose(fp);
		return NULL;
	}

	//读取调色板
	for (unsigned int nCount = 0; nCount < imInfo.biClrUsed; nCount++) {
		fread((char *)&(imPla[nCount].rgbBlue), 1, sizeof(BYTE), fp);
		fread((char *)&(imPla[nCount].rgbGreen), 1, sizeof(BYTE), fp);
		fread((char *)&(imPla[nCount].rgbRed), 1, sizeof(BYTE), fp);
		fread((char *)&(imPla[nCount].rgbReserved), 1, sizeof(BYTE), fp);
	}

	width = imInfo.biWidth;
	height = imInfo.biHeight;
	

	if (imInfo.biBitCount == 24) {
		//图像每一行的字节数必须是4的整数倍,windows图像规定
		width = (width * sizeof(BYTE) + 3) / 4 * 4;
		imStruct->width = width;
		imStruct->height = height;
		imStruct->data = (BYTE*)malloc(width * height * 3);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width * 3; j++) {
				*(imStruct->data + i * width + j) = 0;
			}
		}
		fseek(fp, imHead.bfOffBits, SEEK_SET);
		fread(imStruct->data, sizeof(BYTE) * width * 3, height, fp);
		imStruct->chanel = 3; //这里彩色图像直接设置为3通道，作为保留，以后可能用到
		imStruct->type = COLORIMG; 
	}
	else {
		//图像每一行的字节数必须是4的整数倍,windows图像规定
		width = (width * sizeof(BYTE) + 3) / 4 * 4;
		imStruct->width = width;
		imStruct->height = height;
		imStruct->data = (BYTE*)malloc(width * height);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				*(imStruct->data + i * width + j) = 0;
			}
		}
		//imHead.bfOffBits = 54;
		fseek(fp, imHead.bfOffBits, SEEK_SET);
		//cout << sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) << endl;
		fread(imStruct->data, sizeof(BYTE) * width, height, fp);
		imStruct->chanel = 1;
		imStruct->type = GRAY;
	}
	imStruct->deep = imInfo.biBitCount;

	fclose(fp);

	return imStruct;
}

//写灰度图
bool WriteGrayImage(ImStruct* img, const char* filename) {
	bool SUC = true;
	FILE* fpi;
	BITMAPFILEHEADER imHead; //位图头
	BITMAPINFOHEADER imInfo; //位图头文件信息
	BYTE p[4];

	//cout << img->height << endl;
	if ((fpi = fopen(filename, "w+b")) == NULL) {
		cout << "file open fail" << endl;
		return false;
	}
	//cout << img->height << endl;
	imHead.bfType = (WORD)0x4d42;
	imHead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * 4L;
	imHead.bfSize = imHead.bfOffBits + img->width * img->height;
	imHead.bfReserved1 = 0;
	imHead.bfReserved2 = 0;
	if (fwrite(&imHead, 1, sizeof(imHead), fpi) != sizeof(imHead)) SUC = false;

	imInfo.biSize = 40;
	imInfo.biWidth = img->width;
	imInfo.biHeight = img->height;
	imInfo.biPlanes = 1;
	imInfo.biBitCount = 8;
	imInfo.biCompression = 0;
	imInfo.biSizeImage = 0;
	imInfo.biXPelsPerMeter = 0;
	imInfo.biYPelsPerMeter = 0;
	imInfo.biClrUsed = 256;
	imInfo.biClrImportant = 0;
	if (fwrite(&imInfo, 1, sizeof(imInfo), fpi) != sizeof(imInfo)) SUC = false;
	
	for (int i = 0; i < 256; i++) {
		p[3] = 0;
		p[0] = p[1] = p[2] = i;  //RGB
		if (fwrite(&p, 1, 4, fpi) != 4) {
			SUC = false; 
			break;
		}
	}
	//fseek(fpi, imHead.bfOffBits, SEEK_SET);
	//cout << fwrite(img->data, sizeof(BYTE) * img->width, img->height, fpi) << endl;
	if (fwrite(img->data, sizeof(BYTE) * img->width, img->height, fpi) != (img->height)) SUC = false;

	fclose(fpi);

	return SUC;
}

//写彩色图
bool WriteColorImage(ImStruct* img, const char* filename) {
	bool SUC = true;
	FILE* fp;
	BITMAPFILEHEADER imHead; //位图头
	BITMAPINFOHEADER imInfo; //位图头文件信息
	BYTE p[4];

	if ((fp = fopen(filename, "w+b")) == NULL) {
		std::cout << "file open fail" << std::endl;
		return false;
	}

	imHead.bfType = (WORD)0x4d42;
	imHead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	imHead.bfSize = imHead.bfOffBits + img->width * img->height;
	imHead.bfReserved1 = 0;
	imHead.bfReserved2 = 0;
	if (fwrite(&imHead, 1, sizeof(imHead), fp) != sizeof(imHead)) SUC = false;

	imInfo.biSize = 40;
	imInfo.biWidth = img->width;
	imInfo.biHeight = img->height;
	imInfo.biPlanes = 1;
	imInfo.biBitCount = 24;
	imInfo.biCompression = 0;
	imInfo.biSizeImage = 0;
	imInfo.biXPelsPerMeter = 0;
	imInfo.biYPelsPerMeter = 0;
	imInfo.biClrUsed = 0;
	imInfo.biClrImportant = 0;
	if (fwrite(&imInfo, 1, sizeof(imInfo), fp) != sizeof(imInfo)) SUC = false;

	if (fwrite(img->data, sizeof(BYTE) * img->width * 3, img->height, fp) != (img->width*img->height * 3)) SUC = false;

	fclose(fp);

	return SUC;
}

//写位图文件
bool WriteImage(ImStruct* img, const char* filename) {
	bool SUC = true;
	if (img->type == GRAY) {
		SUC = WriteGrayImage(img, filename);
	}
	else if (img->type == COLORIMG) {
		SUC = WriteColorImage(img, filename);
	}
	else {
		SUC = false;
	}

	return SUC;
}