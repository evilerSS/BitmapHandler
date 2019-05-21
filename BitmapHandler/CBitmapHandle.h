#pragma once
#include "CMyBitmap.h"

class CBitmapHandle
{
	// Cumulative Distribution Function: 累计分布函数
	// 输入：灰度级出现的个数以及累加概率
	// 输出：处理后的灰度值
	static int CDF(int n, float k); 
public:
	CBitmapHandle();
	//将文件图片fileName中的信息转存储到myBitmap中并返回CMyBitmap对象指针
	CMyBitmap*  ReadBitmapFile(char *fileName);
	//分离三原色并输出图片
	bool SeparateRGB(CMyBitmap* originaLBmp, char* outputFileNameR, char* outputFileNameG, char* outputFileNameB);
	//位图灰度化(8位)并输出
	bool GrayscaleBitmap(CMyBitmap* originaLBmp, char* outputFileName);
	//(8位)灰度图反色并输出
	bool GrayscaleAntiColor(CMyBitmap* originaLBmp, char* outputFileName);
	//8位灰度位图的直方图
	bool Histogram(CMyBitmap* originaLBmp, char* outputFileName);
	//直方图均衡化
	bool HistogramEqualization(CMyBitmap* originaLBmp, char* outputFileName);
	// 平均处理（基于模板卷积运算）（处理边界点）
	bool AverageHandleWithBorder(CMyBitmap* originaLBmp, char* outputFileName);
	// 平均处理（基于模板卷积运算）（不处理边界点）
	bool AverageHandleWithoutBorder(CMyBitmap* originaLBmp, char* outputFileName);
	// 中值滤波（处理边界点）
	bool MedianFilterWithBorder(CMyBitmap* originaLBmp, char* outputFileName);
	// 中值滤波（不处理边界点）
	bool MedianFilterWithoutBorder(CMyBitmap* originaLBmp, char* outputFileName);
	// 图像缩放
	bool BitmapScale(CMyBitmap* originaLBmp, char* outputFileName, float x, float y);
	// 图像平移
	bool BitmapTranslation(CMyBitmap* originaLBmp, char* outputFileName, int x, int y);
	// 图像镜像
	bool BitmapMirror(CMyBitmap* originaLBmp, char* outputFileName);
	// 图像旋转
	bool BitmapRotate(CMyBitmap* originaLBmp, char* outputFileName, int angle);
	// 阈值分割：给定T
	bool ThresholdDivisionT(CMyBitmap* originaLBmp, char* outputFileName, int t);
	// 阈值分割：迭代阈值法
	bool ThresholdDivisionIteration(CMyBitmap* originaLBmp, char* outputFileName);
	// 阈值分割：Otsu
	bool ThresholdDivisionOtsu(CMyBitmap* originaLBmp, char* outputFileName);
	// 阈值计算迭代函数
	int IterationT(int *grayNum, int oldT);
	// 基于种子点进行区域增长
	bool RegionGrowingWithSeed(CMyBitmap* originaLBmp, char* outputFileName, POINT seedPoint, int t);
	// 无种子点的区域增长
	bool RegionGrowingWithoutSeed(CMyBitmap* originaLBmp, char* outputFileName);
	// 判断是否满足一致性谓词
	//bool isSatisfyConsistency(BYTE* data, int width, int height);
};

