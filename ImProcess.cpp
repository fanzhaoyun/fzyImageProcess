#include"ImProcess.h"

/*
����޸�ʱ�䣺2017��3��27��
*/

/*
���룺
src:ԭͼ��
dst��Ŀ��ͼ��
k������ϵ��
b������ƫ��
�����
������
��ͼ��������Ա任���任֮��д��dst��
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

/*
���룺
src:ԭͼ��
dst��Ŀ��ͼ��
thresh���ֶ���ֵ��ÿ����ֵ������������ޣ���[a,b]����ֵ����ȡ�ռ�����threshparaһһ��Ӧ
threshpara���ֶα任��������[k,b],�ֱ�Ϊ����ϵ��������ƫ�ƣ���threshһһ��Ӧ
�����
������
��ͼ����зֶ����Ա任���任֮��д��dst��
*/
void segmentLine(const ImStruct *src, ImStruct *dst, vector<pair<int,int> > thresh, vector<pair<double,double> > threshpara) {
	BYTE LUT[256];
	int width, height;
	width = dst->width;
	height = dst->height;
	for (int i = 0; i < thresh.size(); i++) {
		for (int j = thresh[i].first; j <= thresh[i].second; j++) {
			int temp = j * threshpara[i].first + threshpara[j].second;
			LUT[i] = (temp < 0) ? 0 : (temp > 255) ? 255 : temp;  //tempС��0ȡ0������255ȡ255
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
���룺
src:ԭͼ��
hist:ֱ��ͼ
�����
������
��ͼ���ֱ��ͼ���������hist��
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
���룺
src��ԭͼ��
�����
���ֵ
������
�����ֵ
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
���룺
src��ԭͼ��
�����
��Сֵ
������
����Сֵ
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
���룺
src��ԭͼ��
�����
ͼ����ֵ
������
����ֵ
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
���룺
src��ԭͼ��
dst��Ŀ��ͼ��
�����
������
ֱ��ͼ���⻯
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
			dst->data[i * width + j] = LUT[src->data[i * width + j]];   //���⻯���ֵд��dst��
		}
	}
	return;
}

/*
���룺
src��ԭͼ��
�����
ͼ�������
������
Ӧ��ֱ��ͼ������ȡͼ������,�൱��ͼ��ľ�ֵ
brighness = ����ֵ�ĺ�/�ܵ����ظ���
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
���룺
src��ԭͼ��
�����
ͼ��ĶԱȶ�
������
��ȡͼ��ĶԱȶȣ�Ҳ���Ƿ���
brighness = ����ֵ�ĺ�/�ܵ����ظ���
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
���룺
src��ԭͼ��
dst��Ŀ��ͼ��
�����
������
����ͼ������ȺͶԱȶ�
G1 = g - u1  ���ȵ���Ϊ0
G2 = G1 * d0/d1  ���������Ϊd0
G3 = G2 + u0  ��ֵ����Ϊu0
*/
void changeBrightnessAndContrast(const ImStruct *src, ImStruct *dst, double u0, double d0) {
	double u1 = brightness(src);
	double d1 = contrast(src);
	int width, height;
	int LUT[256];
	for (int i = 0; i < 256; i++) {
		int temp = (d0 / d1) * (i - u1) + u0;
		LUT[i] = (temp < 0) ? 0 : (temp > 255) ? 255 : temp;  //tempС��0ȡ0������255ȡ255
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
���룺
src��ԭͼ��
dst��Ŀ��ͼ��
�����
������
��ͼ����з�ɫ�����
*/
void antiColor(const ImStruct *src, ImStruct *dst) {
	line(src, dst, -1, 255);
}

/*
���룺
src��ԭͼ��
dst��Ŀ��ͼ��
W���˲����ڵĿ��
H���˲����ڵĸ߶�
�����
������
��ͼ���ֵ�˲�
��ɫͼ���˲�ʱҪ�ֱ���RGBֵ
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
		//��һ�μ���ÿ�еĺ�
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < H; j++) {
				sumtemp[i] += src->data[j * width + i];
			}
		}
		for (; y < (height - H / 2); y++) {
			sum[0] = sum[1] = sum[2] = 0;
			//ÿ��W * H��Χ�ڵĺ�
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
						sum[i] = sum[i] - sumtemp[(x - W / 2)*3 + i] + sumtemp[(x + W / 2 + 1)*3 + i];   //�����˲����ڵ�ֵ
					}
				}
			}
			//����ÿ�еĺ�
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
		//��һ�μ���ÿ�еĺ�
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < H; j++) {
				sumtemp[i] += src->data[j * width + i];
			}
		}
		for (; y < (height - H / 2); y++) {
			sum = 0;
			//ÿ��W * H��Χ�ڵĺ�
			for (int j = 0; j < H; j++) {
				sum  += sumtemp[j];
			}
			int temp2 = (width - W / 2);
			for (x = W / 2; x < temp2; x++) {
				dst->data[y * width + x] = sum / area;
				if (x != (temp2 -1)) {
					sum = sum - sumtemp[x - W / 2] + sumtemp[x + W / 2 + 1];   //�����˲����ڵ�ֵ
				}
			}
			//����ÿ�еĺ�
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
���룺
src��ԭͼ��
dst��Ŀ��ͼ��
radius: �˲��뾶
�����
������
��ͼ����и�˹�˲�
��ɫͼ���˲�ʱҪ�ֱ���RGBֵ
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
		//����sigma���ɸ�˹��
	}
	//��˹�˲�
	
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
���룺
hist��ֱ��ͼ
�����
��ֵ
������
����ֱ��ͼ����ֵ
*/
int getmiddle(int *hist, int imgsize) {
	for (int i = 0, sum = 0; i < 256; i++) {
		sum += hist[i];
		if (sum * 2 > imgsize) return i;
	}
	return 0;
}

/*
���룺
src��ԭͼ��
dst��Ŀ��ͼ��
W: �˲����ڵĿ��
H: �˲����ڵĸ߶�
�����
������
��ͼ�������ֵ�˲�
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
���룺
hist��ֱ��ͼ
�����
��ֵ
������
����ֱ��ͼ�����ֵ
*/
int getmax(int *hist, int imgsize) {
	for (int i = 255; i > 0; i--) {
		if (hist[i]) return i;
	}
	return 0;
}
/*
���룺
src��ԭͼ��
dst��Ŀ��ͼ��
W: �˲����ڵĿ��
H: �˲����ڵĸ߶�
�����
������
��ͼ��������ֵ�˲�
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
���룺
hist��ֱ��ͼ
�����
��ֵ
������
����ֱ��ͼ����Сֵ
*/
int getmin(int *hist, int imgsize) {
	for (int i = 0; i < 256; i++) {
		if (hist[i]) return i;
	}
	return 255;
}
/*
���룺
src��ԭͼ��
dst��Ŀ��ͼ��
W: �˲����ڵĿ��
H: �˲����ڵĸ߶�
�����
������
��ͼ�������Сֵ�˲�
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