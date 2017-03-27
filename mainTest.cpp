#include"readImage.h"
#include"ImProcess.h"

using namespace std;
int main() {

	ImStruct imgStruct,dstImg;
	ImStruct* img = ReadImage(&imgStruct,"D:\\tmp\\s1.bmp");
	dstImg.create(img->width,img->height,GRAY);
	//line(img, &dstImg, 1, 0);
	//dstImg = *img;
	line(img, img, 2, 20);
	if (!WriteImage(img, "D:\\tmp\\s3.bmp")) {
		cout << "Ð´Ê§°Ü" << endl;
	}
	getchar();
	return 0;
}