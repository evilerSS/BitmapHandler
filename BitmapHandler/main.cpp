#include <Iostream>
#include <stdio.h>
#include "CMyBitmap.h"
#include "CBitmapHandle.h"
using namespace std;

int main() {
	char fileNameOriginal[] = "./bitmaps/test.bmp";	//要处理的原图像
	char fileNameR[] = "./bitmaps/bitmap_r.bmp";		//分离rgb中的R形成的图像
	char fileNameG[] = "./bitmaps/bitmap_g.bmp";		//分离rgb中的G形成的图像
	char fileNameB[] = "./bitmaps/bitmap_b.bmp";		//分离rgb中的b形成的图像
	char fileNameGray[] = "./bitmaps/bitmap_gray.bmp";	//8位灰度图像
	char fileNameAntiColor[] = "./bitmaps/bitmap_gray_anti_color.bmp";//8位灰度反色图像
	char fileNameHistogram[] = "./bitmaps/bitmap_histogram.bmp";//8位灰度图的直方图
	char fileNameHistogram2[] = "./bitmaps/bitmap_histogram2.bmp";//8位灰度图的均衡化后的直方图
	char fileNameEqualization[] = "./bitmaps/bitmap_equalization.bmp";//8位灰度图的均衡化
	CMyBitmap		*myBitmap, *myGrayBitmap,*myGrayBitmap2;		//要处理的图像对象
	CBitmapHandle	myBitmapHandler;					//图像处理工具对象

	//解析位图信息并返回解析结果(CMyBitmap对象指针)
	myBitmap = myBitmapHandler.ReadBitmapFile(fileNameOriginal);
	if (myBitmap != NULL) {
		//位图分离三原色并输出
		myBitmapHandler.SeparateRGB(myBitmap, fileNameR, fileNameG, fileNameB);
		//位图灰度化并输出
		myBitmapHandler.GrayscaleBitmap(myBitmap, fileNameGray);
	}
	//解析灰度位图信息并返回解析结果
	myGrayBitmap = myBitmapHandler.ReadBitmapFile(fileNameGray);
	if (myGrayBitmap != NULL) {
		//灰度图反色并输出
		myBitmapHandler.GrayscaleAntiColor(myGrayBitmap, fileNameAntiColor);
		//灰度图直方图输出
		myBitmapHandler.Histogram(myGrayBitmap, fileNameHistogram);
		//灰度图均衡化
		myBitmapHandler.HistogramEqualization(myGrayBitmap, fileNameEqualization);
	}
	//解析均衡化后的灰度图
	myGrayBitmap2 = myBitmapHandler.ReadBitmapFile(fileNameEqualization);
	if (myGrayBitmap2 != NULL) {
		//解析均衡化后的灰度图直方图输出
		myBitmapHandler.Histogram(myGrayBitmap2, fileNameHistogram2);
	}
	system("pause");
	return 0;
}

