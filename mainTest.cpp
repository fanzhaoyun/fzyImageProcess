
#include"readImage.h"
#include"ImProcess.h"

using namespace std;
int main() {

	ImStruct imgStruct,dstImg;
	ImStruct* img = ReadImage(&imgStruct,"D:\\tmp\\s1.bmp");
	dstImg.create(img->width,img->height,GRAY);
	//line(img, &dstImg, 1, 0);
	//dstImg = *img;
	//line(img, &dstImg,1,0);
	minValueSmooth(img, &dstImg);
	if (!WriteImage(&dstImg, "D:\\tmp\\s0.bmp")) {
		cout << "Ð´Ê§°Ü" << endl;
	}
	else {
	//	system("mspaint D:\\tmp\\s3.bmp");
	}
	getchar();
	return 0;
}