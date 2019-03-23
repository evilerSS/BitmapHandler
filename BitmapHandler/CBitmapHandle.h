#pragma once
#include "CMyBitmap.h"

class CBitmapHandle
{
public:
	CBitmapHandle();
	//将文件图片fileName中的信息转存储到myBitmap中并返回CMyBitmap对象指针
	CMyBitmap*  ReadBitmapFile(char *fileName);
	//分离三原色并输出图片
	bool SeparateRGB(CMyBitmap* OriginaLBmp, char* outputFileNameR, char* outputFileNameG, char* outputFileNameB);
	//位图灰度化(8位)并输出
	bool GrayscaleBitmap(CMyBitmap* OriginaLBmp, char* outputFileName);
	//(8位)灰度图反色并输出
	bool GrayscaleAntiColor(CMyBitmap* OriginaLBmp, char* outputFileName);
};

