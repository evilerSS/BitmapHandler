#include <Iostream>
#include <stdio.h>
#include "CMyBitmap.h"
#include "CBitmapHandle.h"
using namespace std;

int main() {
	char fileNameOriginal[] = "./bitmaps/bitmap1.bmp";	//要处理的原图像1
	char fileNameOriginal2[] = "./bitmaps/bitmap2.bmp";	//要处理的原图像2:有噪声的图像
	char fileNameOriginal3[] = "./bitmaps/bitmap3.bmp";	//要处理的原图像3

	char fileNameR[] = "./bitmaps/1/bitmap_r.bmp";		//分离rgb中的R形成的图像
	char fileNameG[] = "./bitmaps/1/bitmap_g.bmp";		//分离rgb中的G形成的图像
	char fileNameB[] = "./bitmaps/1/bitmap_b.bmp";		//分离rgb中的b形成的图像
	char fileNameGray[] = "./bitmaps/1/bitmap_gray.bmp";	//8位灰度图像
	char fileNameAntiColor[] = "./bitmaps/1/bitmap_gray_anti_color.bmp";//8位灰度反色图像

	char fileNameHistogram[] = "./bitmaps/2/bitmap_histogram.bmp";//8位灰度图的直方图
	char fileNameHistogram2[] = "./bitmaps/2/bitmap_histogram2.bmp";//8位灰度图的均衡化后的直方图
	char fileNameEqualization[] = "./bitmaps/2/bitmap_equalization.bmp";//8位灰度图的均衡化

	char fileNameAverageHandleWithBorder[] = "./bitmaps/3/bitmap_average_handle_with_border.bmp";//平均处理(处理边界)后的图像
	char fileNameAverageHandleWithoutBorder[] = "./bitmaps/3/bitmap_average_handle_without_border.bmp";//平均处理(不处理边界)后的图像
	char fileNameMedianFilterWithBorder[] = "./bitmaps/3/bitmap_median_filter_with_border.bmp";//中值滤波(处理边界)后的图像
	char fileNameMedianFilterWithoutBorder[] = "./bitmaps/3/bitmap_median_filter_without_border.bmp";//中值滤波(不处理边界)后的图像

	char fileNameScale[] = "./bitmaps/4/bitmap_scale.bmp";//缩放
	char fileNameTranslation[] = "./bitmaps/4/bitmap_translation.bmp";//平移
	char fileNameMirror[] = "./bitmaps/4/bitmap_mirror.bmp";//镜像
	char fileNameRotate[] = "./bitmaps/4/bitmap_rotate.bmp";//镜像

	char fileNameThresholdDivisionT[] = "./bitmaps/5/bitmap_threshold_division_t.bmp";
	char fileNameThresholdDivisionIteration[] = "./bitmaps/5/bitmap_threshold_division_interation.bmp";
	char fileNameThresholdDivisionOtsu[] = "./bitmaps/5/bitmap_threshold_division_otsu.bmp";

	//要处理的图像对象
	CMyBitmap		*myBitmap,		//原图1
					*myGrayBitmap,	//灰度图
					*myGrayBitmap2,	//灰度图2（均衡化后的）
					*myGrayBitmap3,	//灰度图2（均衡化后的）
					*noiseBitmap;	//有噪声的图像

	CBitmapHandle	myBitmapHandler;//图像处理工具对象

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
	//解析灰度图
	noiseBitmap = myBitmapHandler.ReadBitmapFile(fileNameOriginal2);
	if (noiseBitmap != NULL) {
		//处理噪声
		myBitmapHandler.AverageHandleWithBorder(noiseBitmap, fileNameAverageHandleWithBorder);
		myBitmapHandler.AverageHandleWithoutBorder(noiseBitmap, fileNameAverageHandleWithoutBorder);
		myBitmapHandler.MedianFilterWithBorder(noiseBitmap, fileNameMedianFilterWithBorder);
		myBitmapHandler.MedianFilterWithoutBorder(noiseBitmap, fileNameMedianFilterWithoutBorder);
		//基础变换
		myBitmapHandler.BitmapScale(noiseBitmap, fileNameScale, 0.8, 1.2);
		myBitmapHandler.BitmapTranslation(noiseBitmap, fileNameTranslation, 100, -100); 
		myBitmapHandler.BitmapMirror(noiseBitmap, fileNameMirror);
		myBitmapHandler.BitmapRotate(noiseBitmap, fileNameRotate, -20);
	}
	//解析均衡化后的灰度图
	myGrayBitmap3 = myBitmapHandler.ReadBitmapFile(fileNameOriginal3);
	if (myGrayBitmap3 != NULL) {
		//阈值分割
		myBitmapHandler.ThresholdDivisionT(myGrayBitmap3, fileNameThresholdDivisionT, 128);
		myBitmapHandler.ThresholdDivisionIteration(myGrayBitmap3, fileNameThresholdDivisionIteration);
		myBitmapHandler.ThresholdDivisionOtsu(myGrayBitmap3, fileNameThresholdDivisionOtsu);
	}
	system("pause");
	return 0;
}

