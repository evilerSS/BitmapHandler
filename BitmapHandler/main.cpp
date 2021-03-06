#include <Iostream>
#include <stdio.h>
#include "CMyBitmap.h"
#include "CBitmapHandle.h"
using namespace std;

int main() {
	char fileNameOriginal[] = "./bitmaps/bitmap1.bmp";	//要处理的原图像1
	char fileNameOriginal2[] = "./bitmaps/bitmap2.bmp";	//要处理的原图像2:有噪声的图像
	char fileNameOriginal3[] = "./bitmaps/bitmap3.bmp";	//要处理的原图像3
	char fileNameOriginal4[] = "./bitmaps/bitmap4.bmp";	//要处理的原图像4
	char fileNameOriginal5[] = "./bitmaps/bitmap5.bmp";	//要处理的原图像5
	char fileNameOriginal6[] = "./bitmaps/bitmap6.bmp";	//要处理的原图像5
	char fileNameOriginal7[] = "./bitmaps/bitmap7.bmp";	//要处理的原图像5

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

	char fileNameThresholdDivisionT[] = "./bitmaps/5/bitmap_threshold_division_t.bmp";//阈值分割：给定T
	char fileNameThresholdDivisionIteration[] = "./bitmaps/5/bitmap_threshold_division_interation.bmp";//阈值分割：迭代阈值法
	char fileNameThresholdDivisionOtsu[] = "./bitmaps/5/bitmap_threshold_division_otsu.bmp";//阈值分割：Otsu

	char fileNameRegionGrowingWithSeed[] = "./bitmaps/6/bitmap_region_growing_with_seed.bmp";//基于种子点进行区域增长
	char fileNameRegionGrowingWithoutSeed[] = "./bitmaps/6/bitmap_region_growing_without_seed.bmp";//无种子点的区域增长

	char fileNameEdgeDetectionPrewitt[] = "./bitmaps/7/bitmap_edge_detection_prewitt.bmp";//边缘检测：LOG
	char fileNameEdgeDetectionSobel[] = "./bitmaps/7/bitmap_edge_detection_sobel.bmp";//边缘检测：LOG
	char fileNameEdgeDetectionLOG[] = "./bitmaps/7/bitmap_edge_detection_log.bmp";//边缘检测：LOG

	char fileNameLineDetectoinHough[] = "./bitmaps/8/bitmap_line_detection_hough.bmp";//直线检测：Hough

	char fileNameConnectedDomainAnalysis[] = "./bitmaps/9/bitmap_connected_domain_analysis.bmp";//连通域分析
	char fileNameOutlineExtract[] = "./bitmaps/9/bitmap_outline_extract.bmp";//轮廓提取
	//要处理的图像对象
	CMyBitmap		*myBitmap,		//原图1
					*myGrayBitmap,	//灰度图
					*myGrayBitmap2,	//灰度图2（均衡化后的）
					*myGrayBitmap3,	//灰度图3（均衡化后的）
					*myGrayBitmap4,	//灰度图4
					*myGrayBitmap5,	//灰度图5
					*noiseBitmap,	//有噪声的图像
					*lineDetection,	//直线检测用的图像
					*areaRemark;	//区域标记与轮廓提取用的图像

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
	//解析要处理的灰度图
	myGrayBitmap3 = myBitmapHandler.ReadBitmapFile(fileNameOriginal3);
	if (myGrayBitmap3 != NULL) {
		//阈值分割
		myBitmapHandler.ThresholdDivisionT(myGrayBitmap3, fileNameThresholdDivisionT, 128);
		myBitmapHandler.ThresholdDivisionIteration(myGrayBitmap3, fileNameThresholdDivisionIteration);
		myBitmapHandler.ThresholdDivisionOtsu(myGrayBitmap3, fileNameThresholdDivisionOtsu);
	}
	// 解析要处理的灰度图
	myGrayBitmap4 = myBitmapHandler.ReadBitmapFile(fileNameOriginal4);
	if (myGrayBitmap4 != NULL) {
		//基于种子点的区域增长
		POINT seedPoint;
		seedPoint.x = 350;
		seedPoint.y = 891;
		myBitmapHandler.RegionGrowingWithSeed(myGrayBitmap4, fileNameRegionGrowingWithSeed, seedPoint, 90);
		myBitmapHandler.RegionGrowingWithoutSeed(myGrayBitmap4, fileNameRegionGrowingWithoutSeed);
	}
	// 解析要处理的灰度图
	myGrayBitmap5 = myBitmapHandler.ReadBitmapFile(fileNameOriginal5);
	if (myGrayBitmap5 != NULL) {
		//边缘检测
		myBitmapHandler.EdgeDetectionPrewitt(myGrayBitmap5, fileNameEdgeDetectionPrewitt);
		myBitmapHandler.EdgeDetectionSobel(myGrayBitmap5, fileNameEdgeDetectionSobel);
		myBitmapHandler.EdgeDetectionLOG(myGrayBitmap5, fileNameEdgeDetectionLOG);
	}

	lineDetection = myBitmapHandler.ReadBitmapFile(fileNameOriginal6);
	if (lineDetection != NULL) {
		myBitmapHandler.LineDetectionHough(lineDetection, fileNameLineDetectoinHough);
	}

	areaRemark = myBitmapHandler.ReadBitmapFile(fileNameOriginal7);
	if (areaRemark != NULL) {
		//区域标记与轮廓提取
		myBitmapHandler.ConnectedDomainAnalysis(areaRemark, fileNameConnectedDomainAnalysis);
		myBitmapHandler.OutlineExtract(areaRemark, fileNameOutlineExtract);
	}
	system("pause");
	return 0;
}

