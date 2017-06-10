#include"ImProcess.h"

/*
最后修改时间：2017年3月27日
*/

/*
输入：
src:原图像
dst：目标图像
k：线性系数
b：线性偏移
输出：
描述：
对图像进行线性变换，变换之后写入dst中
*/
void line(const ImStruct *src, ImStruct *dst, double k, double b) {
	int width, height;
	width = dst->width;
	height = dst->height;
	BYTE LUT[256];
	for (int i = 0; i < 256; i++) {
		int temp = i * k + b;
		LUT[i] = (temp < 0) ? 0 : (temp > 255) ? 255 : temp;  //temp小于0取0，大于255取255
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

/*
输入：
src:原图像
dst：目标图像
thresh：分段阈值，每个阈值段需给出上下限，如[a,b]，阈值区间取闭集，与threshpara一一对应
threshpara：分段变换参数，如[k,b],分别为线性系数和线性偏移，与thresh一一对应
输出：
描述：
对图像进行分段线性变换，变换之后写入dst中
*/
void segmentLine(const ImStruct *src, ImStruct *dst, vector<pair<int,int> > thresh, vector<pair<double,double> > threshpara) {
	BYTE LUT[256];
	int width, height;
	width = dst->width;
	height = dst->height;
	for (int i = 0; i < thresh.size(); i++) {
		for (int j = thresh[i].first; j <= thresh[i].second; j++) {
			int temp = j * threshpara[i].first + threshpara[j].second;
			LUT[i] = (temp < 0) ? 0 : (temp > 255) ? 255 : temp;  //temp小于0取0，大于255取255
		}
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

/*
输入：
src:原图像
hist:直方图
输出：
描述：
求图像的直方图，结果放入hist中
*/
void histogram(const ImStruct *src, int * hist) {
	memset(hist, 0, sizeof(int) * 256);
	int width, height;
	width = src->width;
	height = src->height;
	if (src->type == COLORIMG) {
		width *= 3;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			hist[src->data[i * width + j]]++;
		}
	}
}

/*
输入：
src：原图像
输出：
最大值
描述：
求最大值
*/
int maxValue(const ImStruct *src) {
	int hist[256];
	histogram(src, hist);
	for (int i = 255; i >= 0; i--) {
		if (hist[i] != 0)
			return i;
	}
}

/*
输入：
src：原图像
输出：
最小值
描述：
求最小值
*/
int minValue(const ImStruct *src) {
	int hist[256];
	histogram(src, hist);
	for (int i = 0; i < 256; i++) {
		if (hist[i] != 0)
			return i;
	}
}

/*
输入：
src：原图像
输出：
图像中值
描述：
求中值
*/
int midValue(const ImStruct *src) {
	int hist[256];
	int imgsize = src->width * src->height;
	histogram(src, hist);
	for (int i = 0, sum = 0; i < 256; i++) {
		sum += hist[i];
		if (sum * 2 > imgsize) return i;
	}
	return 0;
}

/*
输入：
src：原图像
dst：目标图像
输出：
描述：
直方图均衡化
*/
void histEqualization(const ImStruct *src, ImStruct *dst) {
	int hist[256];
	int LUT[256],sum,i;
	int width, height;
	histogram(src, hist);
	for (sum = hist[0], LUT[0] = 0, i = 1; i < 256; i++) {
		sum += hist[i];
		LUT[i] = 255 * sum / (src->width * src->height);
	}
	width = src->width;
	height = src->height;
	if (src->type == COLORIMG) {
		width *= 3;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dst->data[i * width + j] = LUT[src->data[i * width + j]];   //均衡化后的值写入dst中
		}
	}
	return;
}

/*
输入：
src：原图像
输出：
图像的亮度
描述：
应用直方图快速求取图像亮度,相当于图像的均值
brighness = 像素值的和/总的像素个数
*/
double brightness(const ImStruct *src) {
	int hist[256];
	int sum = 0;
	int width, height;
	histogram(src, hist);
	for (int i = 0; i < 256; i++) {
		sum += (i * hist[i]);
	}
	width = src->width;
	height = src->height;
	if (src->type == COLORIMG) {
		width *= 3;
	}
	return ((double)sum / (width * height));
}

/*
输入：
src：原图像
输出：
图像的对比度
描述：
求取图像的对比度，也就是方差
brighness = 像素值的和/总的像素个数
*/
double contrast(const ImStruct *src){
	int hist[256];
	double sum = 0.0;
	double bn = brightness(src);
	int width, height;
	histogram(src, hist);
	for (int i = 0; i < 256; i++) {
		sum += ((i - bn) * (i - bn) * hist[i]);
	}
	width = src->width;
	height = src->height;
	if (src->type == COLORIMG) {
		width *= 3;
	}
	return sqrt(sum / (width * height - 1));
}

/*
输入：
src：原图像
dst：目标图像
输出：
描述：
调整图像的亮度和对比度
G1 = g - u1  亮度调整为0
G2 = G1 * d0/d1  均方差调整为d0
G3 = G2 + u0  均值调整为u0
*/
void changeBrightnessAndContrast(const ImStruct *src, ImStruct *dst, double u0, double d0) {
	double u1 = brightness(src);
	double d1 = contrast(src);
	int width, height;
	int LUT[256];
	for (int i = 0; i < 256; i++) {
		int temp = (d0 / d1) * (i - u1) + u0;
		LUT[i] = (temp < 0) ? 0 : (temp > 255) ? 255 : temp;  //temp小于0取0，大于255取255
	}
	width = src->width;
	height = src->height;
	if (src->type == COLORIMG) {
		width *= 3;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dst->data[i * width + j] = LUT[src->data[i * width + j]];
		}
	}
}

/*
输入：
src：原图像
dst：目标图像
输出：
描述：
对图像进行反色后输出
*/
void antiColor(const ImStruct *src, ImStruct *dst) {
	line(src, dst, -1, 255);
}

/*
输入：
src：原图像
dst：目标图像
W：滤波窗口的宽度
H：滤波窗口的高度
输出：
描述：
对图像均值滤波
彩色图像滤波时要分别处理RGB值
*/
void averageSmooth(const ImStruct *src, ImStruct *dst, int W, int H) {
	int width, height,area;
	width = src->width;
	height = src->height;
	area = W * H;
	if (width < W || height < H) return;
	if (src->type == COLORIMG) {
		int index = 0;
		width *= 3;
		memcpy(dst->data, src->data, width*height);
		int *sumtemp = new int[width];
		int x = W / 2;
		int y = H / 2;
		int sum[3] = { 0,0,0 };
		memset(sumtemp, 0, sizeof(int) * width);
		//第一次计算每列的和
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < H; j++) {
				sumtemp[i] += src->data[j * width + i];
			}
		}
		for (; y < (height - H / 2); y++) {
			sum[0] = sum[1] = sum[2] = 0;
			//每个W * H范围内的和
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < H; j++) {
					sum[i] += sumtemp[i + (j * 3)];
					index++;
				}
			}
			int temp2 = (width / 3 - (W / 2));
			index = 0;
			for (x = W / 2; x < temp2; x++) {
				for (int i = 0; i < 3; i++) {
					dst->data[y * width + x * 3 + i] = sum[i] / area;
					if (x != (temp2 - 1)) {
						sum[i] = sum[i] - sumtemp[(x - W / 2)*3 + i] + sumtemp[(x + W / 2 + 1)*3 + i];   //更新滤波窗口的值
					}
				}
			}
			//更新每列的和
			int temp = (y - H / 2) * width;
			int temp1 = (y + H / 2 + 1) * width;
			for (int i = 0; i < width; i++) {
				if (y != height - H / 2 - 1) {
					sumtemp[i] = sumtemp[i] - src->data[temp + i] + src->data[temp1 + i];
				}
			}
		}
	}
	else {
		memcpy(dst->data, src->data, width*height);
		int *sumtemp = new int[width];
		int x = W / 2;
		int y = H / 2;
		int sum = 0;
		memset(sumtemp, 0, sizeof(int) * width);
		//第一次计算每列的和
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < H; j++) {
				sumtemp[i] += src->data[j * width + i];
			}
		}
		for (; y < (height - H / 2); y++) {
			sum = 0;
			//每个W * H范围内的和
			for (int j = 0; j < H; j++) {
				sum  += sumtemp[j];
			}
			int temp2 = (width - W / 2);
			for (x = W / 2; x < temp2; x++) {
				dst->data[y * width + x] = sum / area;
				if (x != (temp2 -1)) {
					sum = sum - sumtemp[x - W / 2] + sumtemp[x + W / 2 + 1];   //更新滤波窗口的值
				}
			}
			//更新每列的和
			int temp = (y - H / 2) * width;
			int temp1 = (y + H / 2 + 1) * width;
			for (int i = 0; i < width; i++) {
				if (y != height - H / 2 - 1) {
					sumtemp[i] = sumtemp[i] - src->data[temp + i] + src->data[temp1 + i];
				}
			}
		}
	}
}

/*
输入：
src：原图像
dst：目标图像
radius: 滤波半径
输出：
描述：
对图像进行高斯滤波
彩色图像滤波时要分别处理RGB值
*/
void gaussianSmooth(const ImStruct *src, ImStruct *dst, int radius) {
	const int *gasskner = new int[radius*radius];
	int width, height, sum = 0,area = 0;
	width = src->width;
	height = src->height;
	if (width < radius || height < radius) return;
	memcpy(dst->data, src->data, width*height);

	if (radius == 3) {
		gasskner = GAUSSFILTERTHREE;
		area = 16;
	}
	else if (radius == 5) {
		gasskner = GAUSSFILTERFIVE;
		area = 82;
	}
	else if (radius == 7) {
		gasskner = GAUSSFILTERSEVEN;
		area = 16;
	}
	else {
		//根据sigma生成高斯核
	}
	//高斯滤波
	
	for (int i = 0; i <= height - radius; i++) {
		for (int j = 0; j <= width - radius; j++) {
			sum = 0;
			for (int ii = i,m = 0; m < radius; ii++,m++) {
				for (int jj = j, n = 0; n < radius; jj++,n++) {
					sum += (src->data[ii * width + jj] * gasskner[m * radius + n]);
				}
			}
			dst->data[(i + (radius / 2)) * width + j + radius / 2] = sum / area;
		}
	}
}

/*
输入：
hist：直方图
输出：
中值
描述：
根据直方图求中值
*/
int getmiddle(int *hist, int imgsize) {
	for (int i = 0, sum = 0; i < 256; i++) {
		sum += hist[i];
		if (sum * 2 > imgsize) return i;
	}
	return 0;
}

/*
输入：
src：原图像
dst：目标图像
W: 滤波窗口的宽度
H: 滤波窗口的高度
输出：
描述：
对图像进行中值滤波
*/
void middleValueSmooth(const ImStruct *src, ImStruct *dst, int W, int H) {
	int th = (W * H) / 2 + 1,mid = 0,mNum = 0;
	int hist[256];
	int width, height;
	width = src->width;
	height = src->height;
	if (width < W || height < H) return;
	memcpy(dst->data, src->data, width*height);
	for (int i = 0; i <= height - H; i++) {
		memset(hist, 0, sizeof(int) * 256);
		for (int ii = i, m = 0; m < H; ii++, m++) {
			for (int jj = 0; jj < W; jj++) {
				hist[src->data[ii * width + jj]]++;
			}
		}
		mid = getmiddle(hist, W*H);
		dst->data[(i + (H / 2)) * width + W / 2] = mid;
		mNum = 0;
		for (int ii = i, m = 0; m < H; ii++, m++) {
			for (int jj = 0; jj < W; jj++) {
				if (src->data[ii * width + jj] <= mid) mNum++;
			}
		}
		for (int j = 0; j <= width - W - 1; j++) {
			for (int ii = i, m = 0; m < H; ii++, m++) {
				hist[src->data[ii * width + j]]--;
				if (src->data[ii * width + j] <= mid) mNum--;
			}
			for (int ii = i, m = 0; m < H; ii++, m++) {
				hist[src->data[ii * width + j + W]]++;
				if (src->data[ii * width + j + W] <= mid) mNum++;
			}
			if (mNum > th) {
				while (mNum > th) {
					mNum -= hist[mid];
					mid--;
				}
				mid++;
				mNum += hist[mid];
			}
			else {
				while (mNum <= th) {
					mid++;
					mNum += hist[mid];
				}
			}
			dst->data[(i + (H / 2)) * width + j + W / 2 + 1] = mid;
		}
	}
}

/*
输入：
hist：直方图
输出：
中值
描述：
根据直方图求最大值
*/
int getmax(int *hist, int imgsize) {
	for (int i = 255; i > 0; i--) {
		if (hist[i]) return i;
	}
	return 0;
}
/*
输入：
src：原图像
dst：目标图像
W: 滤波窗口的宽度
H: 滤波窗口的高度
输出：
描述：
对图像进行最大值滤波
*/
void maxValueSmooth(const ImStruct *src, ImStruct *dst, int W, int H) {
	int hist[256], max;
	int width, height,imgsize = W * H;
	width = src->width;
	height = src->height;
	if (width < W || height < H) return;
	memcpy(dst->data, src->data, width*height);
	for (int i = 0; i <= height - H; i++) {
		memset(hist, 0, sizeof(int) * 256);
		for (int ii = i, m = 0; m < H; ii++, m++) {
			for (int jj = 0; jj < W; jj++) {
				hist[src->data[ii * width + jj]]++;
			}
		}
		max = getmax(hist, imgsize);
		dst->data[(i + (H / 2)) * width + W / 2] = max;
		for (int j = 0; j <= width - W - 1; j++) {
			for (int ii = i, m = 0; m < H; ii++, m++) {
				hist[src->data[ii * width + j]]--;
			}
			for (int ii = i, m = 0; m < H; ii++, m++) {
				hist[src->data[ii * width + j + W]]++;
			}
			dst->data[(i + (H / 2)) * width + j + W / 2 + 1] = getmax(hist, imgsize);
		}
	}
}

/*
输入：
hist：直方图
输出：
中值
描述：
根据直方图求最小值
*/
int getmin(int *hist, int imgsize) {
	for (int i = 0; i < 256; i++) {
		if (hist[i]) return i;
	}
	return 255;
}
/*
输入：
src：原图像
dst：目标图像
W: 滤波窗口的宽度
H: 滤波窗口的高度
输出：
描述：
对图像进行最小值滤波
*/
void minValueSmooth(const ImStruct *src, ImStruct *dst, int W, int H) {
	int hist[256], max;
	int width, height, imgsize = W * H;
	width = src->width;
	height = src->height;
	if (width < W || height < H) return;
	memcpy(dst->data, src->data, width*height);
	for (int i = 0; i <= height - H; i++) {
		memset(hist, 0, sizeof(int) * 256);
		for (int ii = i, m = 0; m < H; ii++, m++) {
			for (int jj = 0; jj < W; jj++) {
				hist[src->data[ii * width + jj]]++;
			}
		}
		max = getmin(hist, imgsize);
		dst->data[(i + (H / 2)) * width + W / 2] = max;
		for (int j = 0; j <= width - W - 1; j++) {
			for (int ii = i, m = 0; m < H; ii++, m++) {
				hist[src->data[ii * width + j]]--;
			}
			for (int ii = i, m = 0; m < H; ii++, m++) {
				hist[src->data[ii * width + j + W]]++;
			}
			dst->data[(i + (H / 2)) * width + j + W / 2 + 1] = getmin(hist, imgsize);
		}
	}
}