#pragma once

typedef struct SplitStruct {
	unsigned int nWidth;		//这一块图像的宽度
	unsigned int nHeight;		//这一块图像的高度
	unsigned int nOffsetWidth;	//相对源图像的偏移宽度
	unsigned int nOffsetHeight;	//相对源图像数据的便宜高度
};