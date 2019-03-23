#pragma once
#include <windows.h>
class CMyBitmap
{
public:
	BITMAPFILEHEADER	m_file_head;	//文件头
	BITMAPINFOHEADER	m_info_head;	//信息头
	RGBQUAD				*m_rgbquad;		//调色板
	BYTE				*m_factdata;	//位图实际数据
public:
	CMyBitmap();
	~CMyBitmap();
};

