#pragma once
#ifndef IMG_PROCESS_H
#define IMG_PROCESS_H

#include"readImage.h"

void line(const ImStruct *src, ImStruct *dst, double k, double b);  //线性变换
void segmentLine(const ImStruct *src, ImStruct *dst);               //分段线性变换
int maxValue(const ImStruct *src);                                //最大值
int minValue(const ImStruct *src);                               //最小值
void histogram(const ImStruct *src, int * hist);                 //直方图
void histEqualization(const ImStruct *src, ImStruct *dst);          //直方图均衡化
double brightness(const ImStruct *src);              //亮度
double contrast(const ImStruct *src);                //对比度
void changeBrightnessAndContrast(const ImStruct *src, ImStruct *dst, double brightness, double contrast);  //亮度，对比度调节。单独调节时，另一个置0即可
void antiColor(const ImStruct *src, ImStruct *dst);              //反色
void averageSmooth(const ImStruct *src, ImStruct *dst);          //均值滤波
void gaussianSmooth(const ImStruct *src, ImStruct *dst);         //高斯滤波
void middleValueSmooth(const ImStruct *src, ImStruct *dst);      //中值滤波
void maxValueSmooth(const ImStruct *src, ImStruct *dst);         //最大值滤波
void minValueSmooth(const ImStruct *src, ImStruct *dst);         //最小值滤波
void overrunSmooth(const ImStruct *src, ImStruct *dst);          //超限平滑
void kNeighborsSmooth(const ImStruct *src, ImStruct *dst);       //k邻点平均法
void maskSmooth(const ImStruct *src, ImStruct *dst);             //旋转掩模
void swell(const ImStruct *src, ImStruct *dst);                  //膨胀
void etch(const ImStruct *src, ImStruct *dst);                   //腐蚀
void openOperation(const ImStruct *src, ImStruct *dst);          //开运算
void closeOperation(const ImStruct *src, ImStruct *dst);         //闭运算

#endif // !IMG_PROCESS_H
