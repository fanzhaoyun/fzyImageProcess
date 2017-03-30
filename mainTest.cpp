
#include"readImage.h"
#include"ImProcess.h"

using namespace std;
int main() {

	ImStruct imgStruct,dstImg;
	ImStruct* img = ReadImage(&imgStruct,"D:\\tmp\\dune.bmp");
	dstImg.create(img->width,img->height,COLORIMG);
	//line(img, &dstImg, 1, 0);
	//dstImg = *img;
	//line(img, &dstImg,1,0);
	averageSmooth(img, &dstImg,7,7);
	if (!WriteImage(&dstImg, "D:\\tmp\\dune0.bmp")) {
		cout << "Ð´Ê§°Ü" << endl;
	}
	else {
	//	system("mspaint D:\\tmp\\s3.bmp");
	}
	getchar();
	return 0;
}