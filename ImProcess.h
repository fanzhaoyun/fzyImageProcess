#pragma once
#ifndef IMG_PROCESS_H
#define IMG_PROCESS_H

#include"readImage.h"

void line(const ImStruct *src, ImStruct *dst, double k, double b);  //���Ա任
void segmentLine(const ImStruct *src, ImStruct *dst);               //�ֶ����Ա任
int maxValue(const ImStruct *src);                                //���ֵ
int minValue(const ImStruct *src);                               //��Сֵ
void histogram(const ImStruct *src, int * hist);                 //ֱ��ͼ
void histEqualization(const ImStruct *src, ImStruct *dst);          //ֱ��ͼ���⻯
double brightness(const ImStruct *src);              //����
double contrast(const ImStruct *src);                //�Աȶ�
void changeBrightnessAndContrast(const ImStruct *src, ImStruct *dst, double brightness, double contrast);  //���ȣ��Աȶȵ��ڡ���������ʱ����һ����0����
void antiColor(const ImStruct *src, ImStruct *dst);              //��ɫ
void averageSmooth(const ImStruct *src, ImStruct *dst);          //��ֵ�˲�
void gaussianSmooth(const ImStruct *src, ImStruct *dst);         //��˹�˲�
void middleValueSmooth(const ImStruct *src, ImStruct *dst);      //��ֵ�˲�
void maxValueSmooth(const ImStruct *src, ImStruct *dst);         //���ֵ�˲�
void minValueSmooth(const ImStruct *src, ImStruct *dst);         //��Сֵ�˲�
void overrunSmooth(const ImStruct *src, ImStruct *dst);          //����ƽ��
void kNeighborsSmooth(const ImStruct *src, ImStruct *dst);       //k�ڵ�ƽ����
void maskSmooth(const ImStruct *src, ImStruct *dst);             //��ת��ģ
void swell(const ImStruct *src, ImStruct *dst);                  //����
void etch(const ImStruct *src, ImStruct *dst);                   //��ʴ
void openOperation(const ImStruct *src, ImStruct *dst);          //������
void closeOperation(const ImStruct *src, ImStruct *dst);         //������

#endif // !IMG_PROCESS_H
