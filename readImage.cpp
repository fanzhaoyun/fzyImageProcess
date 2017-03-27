/////////////////////////////////////////////////////////////////////////
//
//          2017.3.21 FZY
//
////////////////////////////////////////////////////////////////////////
#include"readImage.h"


/*
//λͼ�ļ�ͷ����;
//���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������
//Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��
typedef struct  tagBITMAPFILEHEADER{
WORD bfType;//�ļ����ͣ�������0x424D�����ַ���BM��
DWORD bfSize;//�ļ���С
WORD bfReserved1;//������
WORD bfReserved2;//������
DWORD bfOffBits;//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
DWORD biSize;//��Ϣͷ��С
LONG biWidth;//ͼ����
LONG biHeight;//ͼ��߶�
WORD biPlanes;//λƽ����������Ϊ1
WORD biBitCount;//ÿ����λ��
DWORD  biCompression; //ѹ������
DWORD  biSizeImage; //ѹ��ͼ���С�ֽ���
LONG  biXPelsPerMeter; //ˮƽ�ֱ���
LONG  biYPelsPerMeter; //��ֱ�ֱ���
DWORD  biClrUsed; //λͼʵ���õ���ɫ����
DWORD  biClrImportant; //��λͼ����Ҫ��ɫ����
}BITMAPINFOHEADER; //λͼ��Ϣͷ����

typedef struct tagRGBQUAD{
BYTE rgbBlue; //����ɫ����ɫ����
BYTE rgbGreen; //����ɫ����ɫ����
BYTE rgbRed; //����ɫ�ĺ�ɫ����
BYTE rgbReserved; //����ֵ
}RGBQUAD;//��ɫ�嶨��

*/

ImStruct* ReadImage(ImStruct* imStruct,const char* filename)
{
	BITMAPFILEHEADER imHead; //λͼͷ
	BITMAPINFOHEADER imInfo; //λͼͷ�ļ���Ϣ
	RGBQUAD imPla[256]; //256ɫ��ɫ��
	FILE *fp;
	int width, height;
	if ((fp = fopen(filename, "rb")) == NULL){
		std::cout << "file open fail" << std::endl;
		return NULL;
	}
//	WORD bfType;
//	fread(&bfType, 1, sizeof(WORD), fp); //��ȡ�ļ�����
	
	//��ȡ�ļ�ͷ����Ϣͷ
	fread(&imHead, 1, sizeof(BITMAPFILEHEADER), fp);
	fread(&imInfo, 1, sizeof(BITMAPINFOHEADER), fp);
	if (0x4d42 != imHead.bfType) { //λͼ�ļ�ͷ���"BM" 
		std::cout << "This file is not a bmp file" << std::endl;
		fclose(fp);
		return NULL;
	}

	//��ȡ��ɫ��
	for (unsigned int nCount = 0; nCount < imInfo.biClrUsed; nCount++) {
		fread((char *)&(imPla[nCount].rgbBlue), 1, sizeof(BYTE), fp);
		fread((char *)&(imPla[nCount].rgbGreen), 1, sizeof(BYTE), fp);
		fread((char *)&(imPla[nCount].rgbRed), 1, sizeof(BYTE), fp);
		fread((char *)&(imPla[nCount].rgbReserved), 1, sizeof(BYTE), fp);
	}

	width = imInfo.biWidth;
	height = imInfo.biHeight;
	

	if (imInfo.biBitCount == 24) {
		//ͼ��ÿһ�е��ֽ���������4��������,windowsͼ��涨
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
		imStruct->chanel = 3; //�����ɫͼ��ֱ������Ϊ3ͨ������Ϊ�������Ժ�����õ�
		imStruct->type = COLORIMG; 
	}
	else {
		//ͼ��ÿһ�е��ֽ���������4��������,windowsͼ��涨
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

//д�Ҷ�ͼ
bool WriteGrayImage(ImStruct* img, const char* filename) {
	bool SUC = true;
	FILE* fpi;
	BITMAPFILEHEADER imHead; //λͼͷ
	BITMAPINFOHEADER imInfo; //λͼͷ�ļ���Ϣ
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

//д��ɫͼ
bool WriteColorImage(ImStruct* img, const char* filename) {
	bool SUC = true;
	FILE* fp;
	BITMAPFILEHEADER imHead; //λͼͷ
	BITMAPINFOHEADER imInfo; //λͼͷ�ļ���Ϣ
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

//дλͼ�ļ�
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