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
};

