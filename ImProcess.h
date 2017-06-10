#pragma once
#ifndef IMG_PROCESS_H
#define IMG_PROCESS_H

#include<vector>

#include"readImage.h"

const int GAUSSFILTERTHREE[3 * 3] = {1,2,1,2,4,2,1,2,1 };
const int GAUSSFILTERFIVE[5 * 5] = { 1,2,3,2,1,2,5,6,5,2,3,6,8,6,3,2,5,6,5,2,1,2,3,2,1};
const int GAUSSFILTERSEVEN[7 * 7] = { 1,2,2,3,2,2,1,1,2,2,3,2,2,1,1,1,1,1,1,1,1,1,2,3,4,5,6,7,1,2,3,4,5,6,7,1,2,3,4,5,6,7,1,2,3,4,5,6,7 };

void line(const ImStruct *src, ImStruct *dst, double k, double b);  //���Ա任
void segmentLine(const ImStruct *src, ImStruct *dst, vector<pair<int, int> > thresh, vector<pair<double, double> >);               //�ֶ����Ա任
int maxValue(const ImStruct *src);                                //���ֵ
int minValue(const ImStruct *src);                               //��Сֵ
int midValue(const ImStruct *src);
void histogram(const ImStruct *src, int * hist);                 //ֱ��ͼ
void histEqualization(const ImStruct *src, ImStruct *dst);          //ֱ��ͼ���⻯
double brightness(const ImStruct *src);              //����
double contrast(const ImStruct *src);                //�Աȶ�
void changeBrightnessAndContrast(const ImStruct *src, ImStruct *dst, double brightness, double contrast);  //���ȣ��Աȶȵ��ڡ���������ʱ����һ����0����
void antiColor(const ImStruct *src, ImStruct *dst);              //��ɫ
void averageSmooth(const ImStruct *src, ImStruct *dst, int W = 3, int H = 3);          //��ֵ�˲�
void gaussianSmooth(const ImStruct *src, ImStruct *dst,int radius = 3);         //��˹�˲�
void middleValueSmooth(const ImStruct *src, ImStruct *dst, int W = 3, int H = 3);      //��ֵ�˲�
void maxValueSmooth(const ImStruct *src, ImStruct *dst, int W = 3, int H = 3);         //���ֵ�˲�
void minValueSmooth(const ImStruct *src, ImStruct *dst, int W = 3, int H = 3);         //��Сֵ�˲�
void overrunSmooth(const ImStruct *src, ImStruct *dst);          //����ƽ��
void kNeighborsSmooth(const ImStruct *src, ImStruct *dst);       //k�ڵ�ƽ����
void maskSmooth(const ImStruct *src, ImStruct *dst);             //��ת��ģ
void swell(const ImStruct *src, ImStruct *dst);                  //����
void etch(const ImStruct *src, ImStruct *dst);                   //��ʴ
void openOperation(const ImStruct *src, ImStruct *dst);          //������
void closeOperation(const ImStruct *src, ImStruct *dst);         //������

#endif // !IMG_PROCESS_H
