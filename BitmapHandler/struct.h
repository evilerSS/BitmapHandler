#pragma once
#include <afxwin.h>
//本文件只为方便查看位图的结构信息
//WORD为无符号16位二进制整数，DWORD为无符号32位二进制整数


//位图文件头(长度为14个字节)
typedef struct tagBITMAPFILEHEADER {
	WORD   	bfType;			//文件类型，必须是“BM”
	DWORD	bfSize;			//文件大小，包含文件头的大小
	WORD	bfReserved1;	//保留字
	WORD	bfReserved2;	//保留字
	DWORD	bfOffBits;		//从文件头到实际位图数据的偏移字节数
} BITMAPFILEHEADER;

//位图信息头
typedef struct tagBITMAPINFOHEADER {
	DWORD   biSize;			//该结构的长度，为40
	LONG	biWidth;		//图像宽度
	LONG	biHeight;		//图像高度
	WORD	biPlanes;		//位平面数，必须为1
	WORD	biBitCount;		//颜色位数
	DWORD   biCompression;	//是否压缩
	DWORD   biSizeImage;	//实际位图数据占用的字节数
	LONG	biXPelsPerMeter;//目标设备水平分辨率
	LONG	biYPelsPerMeter;//目标设备垂直分辨率
	DWORD   biClrUsed;		//实际使用的颜色数
	DWORD   biClrImportant;	//图像中重要的颜色数
} BITMAPINFOHEADER;

//调色板（Palette）
typedef struct tagRGBQUAD {
	BYTE  rgbBlue;   		//该颜色的蓝色分量
	BYTE  rgbGreen;			//该颜色的绿色分量
	BYTE  rgbRed;			//该颜色的红色分量
	BYTE  rgbReserved;		//保留值
} RGBQUAD;